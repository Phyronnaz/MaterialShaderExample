// Copyright Voxel Plugin SAS. All Rights Reserved.

using UnrealBuildTool;

public class MaterialShaderExample : ModuleRules
{
	public MaterialShaderExample(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bUseUnity = false;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"Projects",
			"RenderCore",
		});
	}
}