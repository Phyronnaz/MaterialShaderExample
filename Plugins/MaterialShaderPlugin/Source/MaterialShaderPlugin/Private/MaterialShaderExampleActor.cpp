// Copyright Voxel Plugin SAS. All Rights Reserved.

#include "MaterialShaderExampleActor.h"
#include "MaterialShaderExampleSubsystem.h"
#include "ExampleStaticMeshComponent.h"

AMaterialShaderExampleActor::AMaterialShaderExampleActor()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UExampleStaticMeshComponent>("Mesh");
	RootComponent = StaticMeshComponent;
}

bool AMaterialShaderExampleActor::ShouldTickIfViewportsOnly() const
{
	// Tick in editor
	return true;
}

void AMaterialShaderExampleActor::Tick(float DeltaSeconds)
{
	// Dummy mesh component to ensure nanite materials are registered
	if (StaticMeshComponent->Materials != NewMaterials)
	{
		StaticMeshComponent->Materials = NewMaterials;
		StaticMeshComponent->MarkRenderStateDirty();
	}

	// Hacky but will work fine here

	UMaterialShaderExampleSubsystem* Subsystem = GEngine->GetEngineSubsystem<UMaterialShaderExampleSubsystem>();
	if (!ensure(Subsystem))
	{
		return;
	}

	Subsystem->LocalToWorld = GetActorTransform();
	Subsystem->MaterialSelector = MaterialSelector;
	Subsystem->MaterialToReplace = MaterialToReplace;
	Subsystem->NewMaterials = NewMaterials;
}