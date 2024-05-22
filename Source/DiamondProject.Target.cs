// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class DiamondProjectTarget : TargetRules
{
	public DiamondProjectTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("DiamondProject");

		if(bBuildEditor)
		{
			//ExtraModuleNames.AddRange(new string[] { "CustomLuminariaModule" });
		}
	}
}
