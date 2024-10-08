// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TinyEncryptDemo : ModuleRules
{
	public TinyEncryptDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"UMG",
			"TinyEncrypt"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
