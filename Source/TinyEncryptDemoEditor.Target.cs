// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class TinyEncryptDemoEditorTarget : TargetRules
{
	public TinyEncryptDemoEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
#if UE_5_0_OR_LATER
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
#else
		DefaultBuildSettings = BuildSettingsVersion.V2;
#endif
		ExtraModuleNames.Add("TinyEncryptDemo");
	}
}
