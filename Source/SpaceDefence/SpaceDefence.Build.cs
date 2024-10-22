// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SpaceDefence : ModuleRules
{
    public SpaceDefence(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { 
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "HeadMountedDisplay",
            "UMG",
            "Slate",
            "SlateCore",
            "TextAsset",
            "AIModule",
            "GameplayTasks",
            "NavigationSystem"
        });
    }
}
