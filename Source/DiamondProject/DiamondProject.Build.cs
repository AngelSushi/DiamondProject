// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DiamondProject : ModuleRules
{
	public DiamondProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine",
	        "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput",
	        "Slate", "EngineSettings", "ApplicationCore","UnrealEd","SlateCore","UMG","ProceduralMeshComponent"
        });
    }
}
