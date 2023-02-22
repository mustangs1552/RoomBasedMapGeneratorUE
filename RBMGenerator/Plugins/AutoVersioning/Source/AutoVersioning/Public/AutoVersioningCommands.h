// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "AutoVersioningStyle.h"

class FAutoVersioningCommands : public TCommands<FAutoVersioningCommands>
{
public:

	FAutoVersioningCommands()
		: TCommands<FAutoVersioningCommands>(TEXT("AutoVersioning"), NSLOCTEXT("Contexts", "AutoVersioning", "AutoVersioning Plugin"), NAME_None, FAutoVersioningStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};