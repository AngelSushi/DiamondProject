// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class DiamondProjectEditorTarget : TargetRules
{
	public DiamondProjectEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("DiamondProject");

		ExtraModuleNames.AddRange(new string[] { "CustomLuminariaModule" });
	}
}
