// Copyright Voxel Plugin SAS. All Rights Reserved.

#include "MaterialShaderExampleActor.h"
#include "MaterialShaderExampleSubsystem.h"
#include "ExampleStaticMeshComponent.h"

AMaterialShaderExampleActor::AMaterialShaderExampleActor()
{
	PrimaryActorTick.bCanEverTick = true;

	// Dummy mesh component to ensure nanite materials are registered
	StaticMeshComponent = CreateDefaultSubobject<UExampleStaticMeshComponent>("Mesh");
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetWorldScale3D(FVector::ZeroVector);

	RootComponent = StaticMeshComponent;
}

bool AMaterialShaderExampleActor::ShouldTickIfViewportsOnly() const
{
	// Tick in editor
	return true;
}

void AMaterialShaderExampleActor::Tick(float DeltaSeconds)
{
	for (UMaterialInterface* Material : NewMaterials)
	{
		if (Material)
		{
			Material->CheckMaterialUsage(MATUSAGE_Nanite);
		}
	}

	if (StaticMeshComponent->Materials != NewMaterials)
	{
		StaticMeshComponent->Materials = NewMaterials;
		StaticMeshComponent->MarkRenderStateDirty();
	}

	StaticMeshComponent->SetStaticMesh(DummyMesh);

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