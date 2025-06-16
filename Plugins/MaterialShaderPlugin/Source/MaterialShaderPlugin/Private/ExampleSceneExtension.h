// Copyright Voxel Plugin SAS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneViewExtension.h"

class FExampleSceneExtension : public FSceneViewExtensionBase
{
public:
	using FSceneViewExtensionBase::FSceneViewExtensionBase;

	//~ Begin FSceneViewExtensionBase Interface
	virtual void PreRenderBasePass_RenderThread(
		FRDGBuilder& GraphBuilder,
		bool bDepthBufferIsPopulated) override;
	//~ End FSceneViewExtensionBase Interface
};