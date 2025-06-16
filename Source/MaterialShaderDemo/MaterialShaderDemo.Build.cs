// Copyright Voxel Plugin SAS. All Rights Reserved.

using UnrealBuildTool;

public class MaterialShaderDemo : ModuleRules
{
	public MaterialShaderDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
