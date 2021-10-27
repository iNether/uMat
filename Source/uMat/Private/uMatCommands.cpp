// Copyright Epic Games, Inc. All Rights Reserved.

#include "uMatCommands.h"

#define LOCTEXT_NAMESPACE "FuMatModule"

void FuMatCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "uMat", "Bring up uMat window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
