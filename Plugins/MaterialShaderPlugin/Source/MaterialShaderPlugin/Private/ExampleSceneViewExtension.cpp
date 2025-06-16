// Copyright Voxel Plugin SAS. All Rights Reserved.

#include "ExampleSceneViewExtension.h"
#include "ExampleUtilities.h"
#include "ExampleMaterialShader.h"

#include "ScenePrivate.h"
#include "RenderGraphEvent.h"
#include "RenderGraphBuilder.h"
#include "RenderGraphResources.h"
#include "SceneRendererInterface.h"
#include "PrimitiveUniformShaderParametersBuilder.h"

#include "Materials/MaterialRenderProxy.h"
#include "Rendering/NaniteStreamingManager.h"

// Hack needed because nanite parameters are not exported

#if !IS_MONOLITHIC
const FShaderParametersMetadata* FNaniteShadingUniformParameters::GetStructMetadata()
{
	return FShaderParametersMetadata::GetNameStructMap().FindChecked(FHashedName(TEXT("NaniteShading")));
}

const FShaderParametersMetadata* FNaniteRasterUniformParameters::GetStructMetadata()
{
	return FShaderParametersMetadata::GetNameStructMap().FindChecked(FHashedName(TEXT("NaniteRaster")));
}
#endif

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

FExampleSceneViewExtension::~FExampleSceneViewExtension()
{
	ENQUEUE_RENDER_COMMAND(DestroyExampleSceneExtension)([PrimitiveUniformBuffer = PrimitiveUniformBuffer](FRHICommandList& RHICmdList)
	{
		if (PrimitiveUniformBuffer)
		{
			PrimitiveUniformBuffer->ReleaseResource();
		}
	});
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BEGIN_SHADER_PARAMETER_STRUCT(FMaterialShaderExamplePassParameters,)
	SHADER_PARAMETER_RDG_TEXTURE(Texture2D<UlongType>, VisBuffer64)
	SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<uint>, ShadingMask)

	SHADER_PARAMETER_RDG_BUFFER_SRV(ByteAddressBuffer, ClusterPageData)
	SHADER_PARAMETER_RDG_BUFFER_SRV(ByteAddressBuffer, VisibleClustersSWHW)

	SHADER_PARAMETER_RDG_UNIFORM_BUFFER(FSceneUniformParameters, SceneUniformBuffer)
END_SHADER_PARAMETER_STRUCT()

void FExampleSceneViewExtension::PreRenderBasePass_RenderThread(
	FRDGBuilder& GraphBuilder,
	FSceneView& View,
	bool bDepthBufferIsPopulated)
{
	const FRDGTextureRef VisBuffer64 = FExampleUtilities::FindTexture(GraphBuilder, "Nanite.VisBuffer64");
	const FRDGTextureRef ShadingMask = FExampleUtilities::FindTexture(GraphBuilder, "Nanite.ShadingMask");
	if (!VisBuffer64 ||
		!ShadingMask)
	{
		return;
	}

	RDG_EVENT_SCOPE(GraphBuilder, "MaterialShaderExample");

	const FIntPoint Extent = FIntPoint(
		ShadingMask->Desc.Extent.X,
		ShadingMask->Desc.Extent.Y);

	const FRDGBufferRef VisibleClustersSWHW = FExampleUtilities::FindBuffer(GraphBuilder, "Nanite.VisibleClustersSWHW");
	if (!VisibleClustersSWHW)
	{
		return;
	}

	if (!PrimitiveUniformBuffer)
	{
		FPrimitiveUniformShaderParametersBuilder Builder = FPrimitiveUniformShaderParametersBuilder()
			.Defaults()
			.LocalToWorld(LocalToWorld.ToMatrixWithScale())
			.PreviousLocalToWorld(LocalToWorld.ToMatrixWithScale())
			.ActorWorldPosition(LocalToWorld.GetTranslation());

		PrimitiveUniformBuffer = MakeShared<TUniformBuffer<FPrimitiveUniformShaderParameters>>();
		PrimitiveUniformBuffer->SetContents(GraphBuilder.RHICmdList, Builder.Build());
		PrimitiveUniformBuffer->InitResource(GraphBuilder.RHICmdList);
	}
	check(PrimitiveUniformBuffer);

	FMaterialShaderExamplePassParameters* PassParameters = GraphBuilder.AllocParameters<FMaterialShaderExamplePassParameters>();
	PassParameters->VisBuffer64 = VisBuffer64;
	PassParameters->ShadingMask = GraphBuilder.CreateUAV(ShadingMask);
	PassParameters->ClusterPageData = Nanite::GStreamingManager.GetClusterPageDataSRV(GraphBuilder);
	PassParameters->VisibleClustersSWHW = GraphBuilder.CreateSRV(VisibleClustersSWHW);
	PassParameters->SceneUniformBuffer = GetSceneUniformBufferRef(GraphBuilder, View);

	GraphBuilder.AddPass(
		RDG_EVENT_NAME("Material Shader Example"),
		PassParameters,
		ERDGPassFlags::Compute,
		[WeakThis = SharedThis(this).ToWeakPtr(), &View, Extent, PassParameters](FRHICommandList& RHICmdList)
		{
			if (const TSharedPtr<FExampleSceneViewExtension> This = WeakThis.Pin())
			{
				This->ApplyMaterial(View, Extent, *PassParameters, RHICmdList);
			}
		});
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void FExampleSceneViewExtension::ApplyMaterial(
	const FSceneView& View,
	const FIntPoint& Extent,
	const FMaterialShaderExamplePassParameters& PassParameters,
	FRHICommandList& RHICmdList) const
{
	const FScene& Scene = static_cast<FScene&>(*View.Family->Scene);

	if (!MaterialSelector)
	{
		return;
	}

	const FMaterialRenderProxy* MaterialRenderProxy = MaterialSelector->GetRenderProxy();
	if (!ensure(MaterialRenderProxy))
	{
		return;
	}
	const FMaterial& Material = MaterialRenderProxy->GetMaterialWithFallback(View.GetFeatureLevel(), MaterialRenderProxy);

	const FMaterialShaderMap* MaterialShaderMap = Material.GetRenderingThreadShaderMap();
	if (!ensure(MaterialShaderMap))
	{
		return;
	}

	const TShaderRef<FMaterialShader> Shader = MaterialShaderMap->GetShader<FExampleMaterialShader>();
	if (!ensure(Shader.IsValid()))
	{
		return;
	}

	FRHIComputeShader* ComputeShader = Shader.GetComputeShader();
	if (!ensure(ComputeShader))
	{
		return;
	}

	int32 MaxStreamingPages;
	{
		static IConsoleVariable* CVarStreamingPoolSize = IConsoleManager::Get().FindConsoleVariable(TEXT("r.Nanite.Streaming.StreamingPoolSize"));
		check(CVarStreamingPoolSize);

		int32 StreamingPoolSizeInMB = 0;
		CVarStreamingPoolSize->GetValue(StreamingPoolSizeInMB);

		MaxStreamingPages = (uint64(StreamingPoolSizeInMB) << 20) >> NANITE_STREAMING_PAGE_GPU_SIZE_BITS;
	}

	TUniformBufferRef<FNaniteShadingUniformParameters> NaniteShadingParameters;
	{
		FNaniteShadingUniformParameters Parameters;
		Parameters.ClusterPageData = PassParameters.ClusterPageData;
		Parameters.VisibleClustersSWHW = PassParameters.VisibleClustersSWHW;

		///////////////////////////////////////////////////////////////////////////////////////////
		// TRICKY, MIGHT CAUSE ISSUES IN FUTURE ENGINE RELEASES: Dummy values to pass validation //
		///////////////////////////////////////////////////////////////////////////////////////////

		Parameters.HierarchyBuffer = PassParameters.VisibleClustersSWHW;
		Parameters.ShadingMask = PassParameters.VisBuffer64;
		Parameters.VisBuffer64 = PassParameters.VisBuffer64;
		Parameters.DbgBuffer64 = PassParameters.VisBuffer64;
		Parameters.DbgBuffer32 = PassParameters.VisBuffer64;
		Parameters.ShadingBinData = PassParameters.VisibleClustersSWHW;
		Parameters.MultiViewIndices = PassParameters.VisibleClustersSWHW;
		Parameters.MultiViewRectScaleOffsets = PassParameters.VisibleClustersSWHW;
		Parameters.InViews = PassParameters.VisibleClustersSWHW;

		NaniteShadingParameters = CreateUniformBufferImmediate(Parameters, UniformBuffer_SingleFrame);
	};

	TUniformBufferRef<FNaniteRasterUniformParameters> NaniteRasterParameters;
	{
		FNaniteRasterUniformParameters Parameters;
		Parameters.PageConstants = FIntVector4(0, MaxStreamingPages, 0, 0);
		NaniteRasterParameters = CreateUniformBufferImmediate(Parameters, UniformBuffer_SingleFrame);
	}

	TUniformBufferRef<FExampleMaterialShaderParameters> VoxelParameters;
	{
		// Build a map of render proxy to nanite shading bin
		// Note that this requires the materials we want to access to be added to the scene through their own component
		TMap<const FMaterialRenderProxy*, FNaniteShadingBin> MaterialToShadingBin;
		{
			const FNaniteShadingPipelineMap& ShadingPipelineMap = Scene.NaniteShadingPipelines[ENaniteMeshPass::BasePass].GetShadingPipelineMap();

			MaterialToShadingBin.Reserve(ShadingPipelineMap.Num());

			for (const auto& It : ShadingPipelineMap)
			{
				if (!ensure(It.Value.ShadingPipeline))
				{
					continue;
				}
				FNaniteShadingPipeline& ShadingPipeline = *It.Value.ShadingPipeline;

				const Experimental::FHashElementId ShadingBinId = ShadingPipelineMap.FindId(ShadingPipeline);
				if (!ensure(ShadingBinId.IsValid()))
				{
					continue;
				}

				if (const FNaniteShadingBin* ExistingShadingBin = MaterialToShadingBin.Find(ShadingPipeline.MaterialProxy))
				{
					const FNaniteShadingPipeline& ExistingShadingPipeline = ShadingPipelineMap.GetByElementId(Experimental::FHashElementId(ExistingShadingBin->BinId)).Key;
					ensure(ExistingShadingPipeline.MaterialProxy == ShadingPipeline.MaterialProxy);
					ensure(ExistingShadingPipeline.Material == ShadingPipeline.Material);
					ensure(ExistingShadingPipeline.ComputeShader == ShadingPipeline.ComputeShader);
					continue;
				}

				MaterialToShadingBin.Add(ShadingPipeline.MaterialProxy) = FNaniteShadingBin
				{
					ShadingBinId.GetIndex(),
					It.Value.BinIndex
				};
			}
		}

		const auto GetShadingBin = [&](const UMaterialInterface* MaterialInterface) -> int32
		{
			if (!MaterialInterface)
			{
				return -1;
			}

			const FMaterialRenderProxy* MaterialProxy = MaterialInterface->GetRenderProxy();
			if (!MaterialProxy)
			{
				return -1;
			}

			const FNaniteShadingBin* ShadingBin = MaterialToShadingBin.Find(MaterialProxy);
			if (!ShadingBin)
			{
				return -1;
			}

			return ShadingBin->BinIndex;
		};

		FExampleMaterialShaderParameters Parameters;
		Parameters.ShadingBinToReplace = GetShadingBin(MaterialToReplace);

		for (int32 Index = 0; Index < 16; Index++)
		{
			GET_SCALAR_ARRAY_ELEMENT(Parameters.MaterialIndexToShadingBin, Index) =
				NewMaterials.IsValidIndex(Index)
				? GetShadingBin(NewMaterials[Index])
				: -1;
		}

		Parameters.VisBuffer64 = PassParameters.VisBuffer64;
		Parameters.ShadingMask = PassParameters.ShadingMask;

		VoxelParameters = CreateUniformBufferImmediate(Parameters, UniformBuffer_SingleFrame);
	}

	FMeshDrawShaderBindings ShaderBindings;

	FMeshProcessorShaders MeshProcessorShaders;
	MeshProcessorShaders.ComputeShader = Shader;
	ShaderBindings.Initialize(MeshProcessorShaders);

	FMeshDrawSingleShaderBindings SingleShaderBindings = ShaderBindings.GetSingleShaderBindings(SF_Compute);

	SingleShaderBindings.Add(Shader->GetUniformBufferParameter<FViewUniformShaderParameters>(), View.ViewUniformBuffer);
	SingleShaderBindings.Add(Shader->GetUniformBufferParameter<FSceneUniformParameters>(), PassParameters.SceneUniformBuffer->GetRHI());
	SingleShaderBindings.Add(Shader->GetUniformBufferParameter<FPrimitiveUniformShaderParameters>(), PrimitiveUniformBuffer->GetUniformBufferRHI());
	SingleShaderBindings.Add(Shader->GetUniformBufferParameter<FExampleMaterialShaderParameters>(), VoxelParameters);

	Shader->GetShaderBindings(
		View.Family->Scene,
		View.GetFeatureLevel(),
		*MaterialRenderProxy,
		Material,
		SingleShaderBindings);

	RHICmdList.SetStaticUniformBuffers(FUniformBufferStaticBindings(
		NaniteRasterParameters,
		NaniteShadingParameters));

	SetComputePipelineState(RHICmdList, ComputeShader);

	ShaderBindings.SetOnCommandList(RHICmdList, ComputeShader);

	RHICmdList.DispatchComputeShader(
		FMath::DivideAndRoundUp(Extent.X, 8),
		FMath::DivideAndRoundUp(Extent.Y, 8),
		1);
}