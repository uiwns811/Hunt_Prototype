// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Hunt_PrototypeEditorTarget : TargetRules
{
	public Hunt_PrototypeEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "Hunt_Prototype", "ArenaBattleSetting" } );
	}
}
