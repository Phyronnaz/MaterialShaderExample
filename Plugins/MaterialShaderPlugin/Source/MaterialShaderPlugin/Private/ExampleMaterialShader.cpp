// Copyright Voxel Plugin SAS. All Rights Reserved.

#include "ExampleMaterialShader.h"
#include "DataDrivenShaderPlatformInfo.h"

IMPLEMENT_GLOBAL_SHADER_PARAMETER_STRUCT(FExampleMaterialShaderParameters, "ExampleMaterialShaderParameters");

FExampleMaterialShader::FExampleMaterialShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
	: FMaterialShader(Initializer)
{
}

bool FExampleMaterialShader::ShouldCompilePermutation(const FMaterialShaderPermutationParameters& Parameters)
{
	if (!IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM6))
	{
		// We don't care about mobile or SM5
		return false;
	}

	if (Parameters.MaterialParameters.bIsSpecialEngineMaterial)
	{
		// Always return true to compile the WorldGridMaterial
		// If we don't we won't have a fallback shader and will crash
		return true;
	}

	// We reuse the VirtualHeightfieldMesh flag, see UMaterialShaderExampleSubsystem::Tick
	return Parameters.MaterialParameters.bIsUsedWithVirtualHeightfieldMesh;
}

void FExampleMaterialShader::ModifyCompilationEnvironment(
	const FMaterialShaderPermutationParameters& Parameters,
	FShaderCompilerEnvironment& OutEnvironment)
{
	OutEnvironment.SetDefine(TEXT("IS_NANITE_PASS"), 1);
	OutEnvironment.SetDefine(TEXT("USE_ANALYTIC_DERIVATIVES"), 1);
	OutEnvironment.SetDefine(TEXT("NANITE_USE_UNIFORM_BUFFER"), 1);
	OutEnvironment.SetDefine(TEXT("NANITE_USE_VIEW_UNIFORM_BUFFER"), 1);
	OutEnvironment.SetDefine(TEXT("NANITE_USE_RASTER_UNIFORM_BUFFER"), 1);
	OutEnvironment.SetDefine(TEXT("NANITE_USE_SHADING_UNIFORM_BUFFER"), 1);
	OutEnvironment.SetDefine(TEXT("VF_SUPPORTS_PRIMITIVE_SCENE_DATA"), 1);

	OutEnvironment.CompilerFlags.Add(CFLAG_ForceDXC);
	OutEnvironment.CompilerFlags.Add(CFLAG_HLSL2021);
}

IMPLEMENT_MATERIAL_SHADER_TYPE(
	,
	FExampleMaterialShader,
	TEXT("/Plugin/MaterialShaderExample/MaterialShaderExample.usf"),
	TEXT("Main"),
	SF_Compute);