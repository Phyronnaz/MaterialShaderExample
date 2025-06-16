// Copyright Voxel Plugin SAS. All Rights Reserved.

using UnrealBuildTool;

public class MaterialShaderDemoTarget : TargetRules
{
	public MaterialShaderDemoTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		ExtraModuleNames.AddRange( new string[] { "MaterialShaderDemo" } );
	}
}
