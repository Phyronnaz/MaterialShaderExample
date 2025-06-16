// Copyright Voxel Plugin SAS. All Rights Reserved.

#include "CoreMinimal.h"
#include "Interfaces/IPluginManager.h"

class FMaterialShaderExampleModule : public IModuleInterface
{
public:
	//~ Begin IModuleInterface Implementation
	virtual void StartupModule() override
	{
		const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin("MaterialShaderExample");
		check(Plugin);

		const FString Path = FPaths::ConvertRelativePathToFull(Plugin->GetBaseDir() / "Shaders");

		AddShaderSourceDirectoryMapping(TEXT("/Plugin/MaterialShaderExample"), Path);
	}
	//~ End IModuleInterface Implementation
};
IMPLEMENT_MODULE(FMaterialShaderExampleModule, MaterialShaderExample)