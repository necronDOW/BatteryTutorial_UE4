// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BatteryCollector : ModuleRules
{
	public BatteryCollector(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "HeadMountedDisplay" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
	}
}
