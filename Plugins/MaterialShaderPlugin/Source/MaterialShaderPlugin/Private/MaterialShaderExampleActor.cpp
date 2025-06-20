// Copyright Voxel Plugin SAS. All Rights Reserved.

#include "MaterialShaderExampleActor.h"
#include "MaterialShaderExampleSubsystem.h"

AMaterialShaderExampleActor::AMaterialShaderExampleActor()
{
	PrimaryActorTick.bCanEverTick = true;

	// Dummy mesh component to ensure nanite materials are registered
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetWorldScale3D(FVector::ZeroVector);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshWithManySections(
		TEXT("/MaterialShaderExample/MeshWithManySections.MeshWithManySections"));

	ensure(MeshWithManySections.Object);
	StaticMeshComponent->SetStaticMesh(MeshWithManySections.Object);

	RootComponent = StaticMeshComponent;
}

bool AMaterialShaderExampleActor::ShouldTickIfViewportsOnly() const
{
	// Tick in editor
	return true;
}

void AMaterialShaderExampleActor::Tick(float DeltaSeconds)
{
	for (int32 Index = 0; Index < NewMaterials.Num(); Index++)
	{
		StaticMeshComponent->SetMaterial(Index, NewMaterials[Index]);
	}

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