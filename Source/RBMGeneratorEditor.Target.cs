// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class RBMGeneratorEditorTarget : TargetRules
{
	public RBMGeneratorEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		
		ExtraModuleNames.AddRange( new string[] { "RBMGenerator" } );
	}
}
