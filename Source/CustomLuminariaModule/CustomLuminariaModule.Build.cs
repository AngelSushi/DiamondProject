// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CustomLuminariaModule : ModuleRules
{
	public CustomLuminariaModule(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange( new string[]{ "Core","DiamondProject","CoreUObject","Engine","UnrealEd","ComponentVisualizers","InputCore" });
    }
}
