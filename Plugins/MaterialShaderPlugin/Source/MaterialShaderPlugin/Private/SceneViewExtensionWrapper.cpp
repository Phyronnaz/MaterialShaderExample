// Copyright Voxel Plugin SAS. All Rights Reserved.

#include "SceneViewExtensionWrapper.h"

void FSceneViewExtensionWrapper::PreRenderViewFamily_RenderThread(
	FRDGBuilder& GraphBuilder,
	FSceneViewFamily& ViewFamily)
{
	ensure(CurrentViews.Num() == 0);
	CurrentViews.Reset();
}

void FSceneViewExtensionWrapper::PreRenderView_RenderThread(
	FRDGBuilder& GraphBuilder,
	FSceneView& View)
{
	check(IsInRenderingThread());
	CurrentViews.Add(&View);
}

void FSceneViewExtensionWrapper::PostRenderView_RenderThread(
	FRDGBuilder& GraphBuilder,
	FSceneView& View)
{
	check(IsInRenderingThread());
	ensure(CurrentViews.Remove(&View) == 1);
}

void FSceneViewExtensionWrapper::PreRenderBasePass_RenderThread(
	FRDGBuilder& GraphBuilder,
	bool bDepthBufferIsPopulated)
{
	check(IsInRenderingThread());
	ensure(CurrentViews.Num() > 0);

	for (FSceneView* View : CurrentViews)
	{
		PreRenderBasePass_RenderThread(GraphBuilder, *View, bDepthBufferIsPopulated);
	}
}