// Copyright Epic Games, Inc. All Rights Reserved.

#include "AutoVersioningCommands.h"

#define LOCTEXT_NAMESPACE "FAutoVersioningModule"

void FAutoVersioningCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "Auto Versioning", "Bring up Auto Versioning window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
