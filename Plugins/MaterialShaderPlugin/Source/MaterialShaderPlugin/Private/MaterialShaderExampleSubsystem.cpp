// Copyright Voxel Plugin SAS. All Rights Reserved.

#include "MaterialShaderExampleSubsystem.h"
#include "ExampleSceneViewExtension.h"

void UMaterialShaderExampleSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	SceneViewExtension = FSceneViewExtensions::NewExtension<FExampleSceneViewExtension>();
}

void UMaterialShaderExampleSubsystem::Tick(float DeltaTime)
{
	if (MaterialSelector)
	{
		// Make sure our material selector material gets compiled for our material shader
		// We reuse the VirtualHeightfieldMesh flag, see FExampleMaterialShader::ShouldCompilePermutation
		MaterialSelector->CheckMaterialUsage(MATUSAGE_VirtualHeightfieldMesh);
	}

	if (!SceneViewExtension)
	{
		return;
	}

	// Copy data to the render thread
	ENQUEUE_RENDER_COMMAND(MaterialShaderExampleSubsystemCopy)([
		SceneViewExtension = SceneViewExtension,
		LocalToWorld = LocalToWorld,
		MaterialSelector = MaterialSelector,
		MaterialToReplace = MaterialToReplace,
		NewMaterials = NewMaterials](FRHICommandList& RHICmdList)
	{
		SceneViewExtension->LocalToWorld = LocalToWorld;
		SceneViewExtension->MaterialSelector = MaterialSelector;
		SceneViewExtension->MaterialToReplace = MaterialToReplace;
		SceneViewExtension->NewMaterials = NewMaterials;
	});
}

TStatId UMaterialShaderExampleSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UMaterialShaderExampleSubsystem, STATGROUP_Tickables);
}

bool UMaterialShaderExampleSubsystem::IsTickableInEditor() const
{
	return true;
}