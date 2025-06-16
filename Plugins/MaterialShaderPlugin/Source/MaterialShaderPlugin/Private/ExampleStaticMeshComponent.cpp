// Copyright Voxel Plugin SAS. All Rights Reserved.

#include "ExampleStaticMeshComponent.h"
#include "NaniteSceneProxy.h"

class FMaterialShaderExampleNaniteSceneProxy : public Nanite::FSceneProxy
{
public:
	FMaterialShaderExampleNaniteSceneProxy(
		const Nanite::FMaterialAudit& MaterialAudit,
		UExampleStaticMeshComponent* Component)
		: FSceneProxy(MaterialAudit, Component)
	{
		MaterialSections.Reset();

		for (const UMaterialInterface* Material : Component->Materials)
		{
			if (!Material)
			{
				Material = UMaterial::GetDefaultMaterial(MD_Surface);
			}

			FMaterialSection& MaterialSection = MaterialSections.Emplace_GetRef();
			MaterialSection.RasterMaterialProxy = Material->GetRenderProxy();
			MaterialSection.ShadingMaterialProxy = Material->GetRenderProxy();
		}
	}
};

FPrimitiveSceneProxy* UExampleStaticMeshComponent::CreateStaticMeshSceneProxy(
	Nanite::FMaterialAudit& NaniteMaterials,
	const bool bCreateNanite)
{
	if (!ensure(bCreateNanite))
	{
		return nullptr;
	}

	return new FMaterialShaderExampleNaniteSceneProxy(NaniteMaterials, this);
}