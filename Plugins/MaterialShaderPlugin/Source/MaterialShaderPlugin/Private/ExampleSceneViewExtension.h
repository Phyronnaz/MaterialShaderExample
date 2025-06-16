// Copyright Voxel Plugin SAS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneViewExtensionWrapper.h"

class FMaterialShaderExamplePassParameters;

class FExampleSceneViewExtension : public FSceneViewExtensionWrapper
{
public:
	FTransform LocalToWorld;

	// Not super safe, but passing materials to the render thread never is
	// We do not want to use a weak pointer, accessing it on the render thread might be unsafe
	UMaterialInterface* MaterialSelector = nullptr;
	UMaterialInterface* MaterialToReplace = nullptr;
	TArray<UMaterialInterface*> NewMaterials;

public:
	using FSceneViewExtensionWrapper::FSceneViewExtensionWrapper;
	virtual ~FExampleSceneViewExtension() override;

	//~ Begin FSceneViewExtensionBase Interface
	virtual void PreRenderBasePass_RenderThread(
		FRDGBuilder& GraphBuilder,
		FSceneView& View,
		bool bDepthBufferIsPopulated) override;
	//~ End FSceneViewExtensionBase Interface

private:
	TSharedPtr<TUniformBuffer<FPrimitiveUniformShaderParameters>> PrimitiveUniformBuffer;

	void ApplyMaterial(
		const FSceneView& View,
		const FIntPoint& Extent,
		const FMaterialShaderExamplePassParameters& PassParameters,
		FRHICommandList& RHICmdList) const;
};
