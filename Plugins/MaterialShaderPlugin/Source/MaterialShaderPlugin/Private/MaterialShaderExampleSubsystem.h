// Copyright Voxel Plugin SAS. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MaterialShaderExampleSubsystem.generated.h"

class FExampleSceneViewExtension;

UCLASS()
class UMaterialShaderExampleSubsystem
	: public UEngineSubsystem
	, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FTransform LocalToWorld;

	UPROPERTY()
	TObjectPtr<UMaterialInterface> MaterialSelector;

	UPROPERTY()
	TObjectPtr<UMaterialInterface> MaterialToReplace;

	UPROPERTY()
	TArray<TObjectPtr<UMaterialInterface>> NewMaterials;

public:
	//~ Begin UEngineSubsystem Interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	//~ End UEngineSubsystem Interface

	//~ Begin FTickableGameObject Interface
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickableInEditor() const override;
	//~ End FTickableGameObject Interface

private:
	TSharedPtr<FExampleSceneViewExtension> SceneViewExtension;
};