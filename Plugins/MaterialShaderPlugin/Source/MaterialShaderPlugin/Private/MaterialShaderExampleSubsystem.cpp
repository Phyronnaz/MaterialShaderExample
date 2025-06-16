// Copyright Voxel Plugin SAS. All Rights Reserved.

#include "MaterialShaderExampleSubsystem.h"
#include "ExampleSceneExtension.h"

void UMaterialShaderExampleSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	SceneViewExtension = FSceneViewExtensions::NewExtension<FExampleSceneExtension>();
}

void UMaterialShaderExampleSubsystem::Tick(float DeltaTime)
{
	if (MaterialSelector)
	{
		// Make sure our material selector material gets compiled for our material shader
		// We reuse the VirtualHeightfieldMesh flag, see FExampleMaterialShader::ShouldCompilePermutation
		MaterialSelector->CheckMaterialUsage(MATUSAGE_VirtualHeightfieldMesh);
	}
}

TStatId UMaterialShaderExampleSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UMaterialShaderExampleSubsystem, STATGROUP_Tickables);
}
