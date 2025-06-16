// Copyright Voxel Plugin SAS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MaterialShaderExampleSubsystem.generated.h"

class FExampleSceneExtension;

UCLASS()
class UMaterialShaderExampleSubsystem
	: public UEngineSubsystem
	, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UMaterialInterface> MaterialToReplace;

	UPROPERTY()
	TObjectPtr<UMaterialInterface> MaterialSelector;

	UPROPERTY()
	TArray<TObjectPtr<UMaterialInterface>> NewMaterials;

public:
	//~ Begin UEngineSubsystem Interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	//~ End UEngineSubsystem Interface

	//~ Begin FTickableGameObject Interface
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	//~ End FTickableGameObject Interface

private:
	TSharedPtr<FExampleSceneExtension> SceneViewExtension;
};