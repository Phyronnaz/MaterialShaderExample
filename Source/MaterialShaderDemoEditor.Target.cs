// Copyright Voxel Plugin SAS. All Rights Reserved.

using UnrealBuildTool;

public class MaterialShaderDemoEditorTarget : TargetRules
{
	public MaterialShaderDemoEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		ExtraModuleNames.AddRange( new string[] { "MaterialShaderDemo" } );
	}
}
