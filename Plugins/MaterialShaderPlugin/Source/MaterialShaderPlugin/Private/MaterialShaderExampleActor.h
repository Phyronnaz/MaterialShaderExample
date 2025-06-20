// Copyright Voxel Plugin SAS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MaterialShaderExampleActor.generated.h"

class UStaticMeshComponent;

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

public:
	AMaterialShaderExampleActor();

	//~ Begin AActor Interface
	virtual bool ShouldTickIfViewportsOnly() const override;
	virtual void Tick(float DeltaSeconds) override;
	//~ End AActor Interface

private:
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
};