// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class TinyEncryptDemoTarget : TargetRules
{
	public TinyEncryptDemoTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
#if UE_5_0_OR_LATER
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
#else
		DefaultBuildSettings = BuildSettingsVersion.V2;
#endif
		ExtraModuleNames.Add("TinyEncryptDemo");
	}
}
