// Copyright Voxel Plugin SAS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "ExampleStaticMeshComponent.generated.h"

UCLASS(NotBlueprintable)
class UExampleStaticMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<TObjectPtr<UMaterialInterface>> Materials;

	//~ Begin UStaticMeshComponent Interface
	virtual FPrimitiveSceneProxy* CreateStaticMeshSceneProxy(
		Nanite::FMaterialAudit& NaniteMaterials,
		bool bCreateNanite) override;
	//~ End UStaticMeshComponent Interface
};