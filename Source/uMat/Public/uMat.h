#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#include "CoreTypes.h"
#include "Templates/Function.h"
#include "Containers/UnrealString.h"
#include "Misc/DateTime.h"
#include "Misc/EnumClassFlags.h"
#include "GenericPlatform/GenericPlatformFile.h"

class FToolBarBuilder;
class FMenuBuilder;

TSharedPtr<SEditableTextBox> data_dirTextBox;

class FuMatModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();

private:

	void RegisterMenus();

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

private:
	TSharedPtr<class FUICommandList> PluginCommands;
};