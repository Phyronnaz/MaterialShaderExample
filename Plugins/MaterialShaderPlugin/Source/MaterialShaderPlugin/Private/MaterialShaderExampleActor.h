// Copyright Voxel Plugin SAS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MaterialShaderExampleActor.generated.h"

class UExampleStaticMeshComponent;

UCLASS()
class AMaterialShaderExampleActor : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Config")
	TObjectPtr<UMaterialInterface> MaterialSelector;

	UPROPERTY(EditAnywhere, Category = "Config")
	TObjectPtr<UMaterialInterface> MaterialToReplace;

	UPROPERTY(EditAnywhere, Category = "Config")
	TArray<TObjectPtr<UMaterialInterface>> NewMaterials;

	// Dummy mesh used to render all our materials
	// This is needed for their shading bins to be registered
	UPROPERTY(EditAnywhere, Category = "Config")
	TObjectPtr<UStaticMesh> DummyMesh;

public:
	AMaterialShaderExampleActor();

	//~ Begin AActor Interface
	virtual bool ShouldTickIfViewportsOnly() const override;
	virtual void Tick(float DeltaSeconds) override;
	//~ End AActor Interface

private:
	UPROPERTY()
	TObjectPtr<UExampleStaticMeshComponent> StaticMeshComponent;
};