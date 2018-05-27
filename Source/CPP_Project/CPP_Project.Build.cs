// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CPP_Project : ModuleRules
{
	public CPP_Project(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore","UMG" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate","SlateCore" });
    
    }
}
