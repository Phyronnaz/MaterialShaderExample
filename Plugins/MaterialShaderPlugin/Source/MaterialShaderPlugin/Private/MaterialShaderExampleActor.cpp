// Copyright Voxel Plugin SAS. All Rights Reserved.

#include "MaterialShaderExampleActor.h"
#include "MaterialShaderExampleSubsystem.h"

AMaterialShaderExampleActor::AMaterialShaderExampleActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

bool AMaterialShaderExampleActor::ShouldTickIfViewportsOnly() const
{
	// Tick in editor
	return true;
}

void AMaterialShaderExampleActor::Tick(float DeltaSeconds)
{
	// Hacky but will work fine here

	UMaterialShaderExampleSubsystem* Subsystem = GEngine->GetEngineSubsystem<UMaterialShaderExampleSubsystem>();
	if (!ensure(Subsystem))
	{
		return;
	}

	Subsystem->MaterialToReplace = MaterialToReplace;
	Subsystem->NewMaterials = NewMaterials;
}