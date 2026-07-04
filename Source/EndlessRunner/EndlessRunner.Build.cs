// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EndlessRunner : ModuleRules
{
	public EndlessRunner(ReadOnlyTargetRules target) : base(target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "ModelViewViewModel", "ModelViewViewModel" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
