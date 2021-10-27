// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "uMatStyle.h"

class FuMatCommands : public TCommands<FuMatCommands>
{
public:

	FuMatCommands()
		: TCommands<FuMatCommands>(TEXT("uMat"), NSLOCTEXT("Contexts", "uMat", "uMat Plugin"), NAME_None, FuMatStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};