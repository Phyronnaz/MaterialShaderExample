// Copyright Voxel Plugin SAS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneViewExtension.h"

// Wrapper to pass View to PreRenderBasePass_RenderThread
class FSceneViewExtensionWrapper : public FSceneViewExtensionBase
{
public:
	using FSceneViewExtensionBase::FSceneViewExtensionBase;

	//~ Begin FSceneViewExtensionBase Interface
	virtual void PreRenderViewFamily_RenderThread(
		FRDGBuilder& GraphBuilder,
		FSceneViewFamily& ViewFamily) override;

	virtual void PreRenderView_RenderThread(
		FRDGBuilder& GraphBuilder,
		FSceneView& View) override;

	virtual void PostRenderView_RenderThread(
		FRDGBuilder& GraphBuilder,
		FSceneView& View) override;

	virtual void PreRenderBasePass_RenderThread(
		FRDGBuilder& GraphBuilder,
		bool bDepthBufferIsPopulated) final override;
	//~ End FSceneViewExtensionBase Interface

public:
	virtual void PreRenderBasePass_RenderThread(
		FRDGBuilder& GraphBuilder,
		FSceneView& View,
		bool bDepthBufferIsPopulated)
	{
	}

private:
	// Multiple views in VR
	TArray<FSceneView*> CurrentViews;
};