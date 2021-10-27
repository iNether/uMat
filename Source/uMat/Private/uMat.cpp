#include "uMat.h"
#include "uMatStyle.h"
#include "uMatCommands.h"
#include "LevelEditor.h"

#include <string>
#include <filesystem>

#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"

#include "ToolMenus.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/FileManager.h"
#include "HAL/FileManagerGeneric.h"
#include "GenericPlatform/GenericPlatformFile.h"

#include "Runtime/JsonUtilities/Public/JsonUtilities.h"

#include "Materials/MaterialInstance.h"
#include "Factories/MaterialInstanceConstantFactoryNew.h"
#include "Materials/MaterialInstanceConstant.h"

static const FName uMatTabName("uMat");

#define LOCTEXT_NAMESPACE "FuMatModule"

void FuMatModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FuMatStyle::Initialize();
	FuMatStyle::ReloadTextures();

	FuMatCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FuMatCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FuMatModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FuMatModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(uMatTabName, FOnSpawnTab::CreateRaw(this, &FuMatModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FuMatTabTitle", "uMat"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FuMatModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FuMatStyle::Shutdown();

	FuMatCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(uMatTabName);
}

TSharedRef<SDockTab> FuMatModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Center)
					[
						SNew(STextBlock)
						.Text(FText::FromString("data_dir"))
					]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Center)
					[
							SAssignNew(data_dirTextBox ,SEditableTextBox)
							.Text(FText::FromString("G:/DataMine/_umap034"))
							.HintText(FText::FromString("directory of the uMap .bat"))
					]
			]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Bottom)
				[
					SNew(SButton)
					.OnClicked_Lambda([]()->FReply {	GLog->Log(FText::FromString("uMat PLUGIN was RUNNED"));
														
														FString data_dir = data_dirTextBox->GetText().ToString()+"/";
														int32 data_dirLen = data_dir.Len();

														GLog->Log("data_dir = " + data_dir);

														FString map_name; 
														FFileHelper::LoadFileToString(map_name, *(data_dir+"processed.json"));
														map_name = map_name.TrimQuotes();
														GLog->Log("map_name = " + map_name);
														
														/*FString processed_map;
														FFileHelper::LoadFileToString(processed_map, *(data_dir.LeftChop(1) + map_name +".processed.json"));
														GLog->Log("processed_map = " + processed_map);*/
																
														TArray<FString> Files;
														FString fileExtension = "*.json";

														IFileManager::Get().FindFilesRecursive(Files, *data_dir, *fileExtension, true, false, false);

														for (int i = 0; i != Files.Num(); i++)
														{
															const FString JsonFilePath = Files[i];

															FString JsonString;
															FFileHelper::LoadFileToString(JsonString, *JsonFilePath);
															FText StatusText = FText::FromString(JsonString);

															TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
															JsonString.RemoveFromStart("[");
															JsonString.RemoveFromEnd("]");

															TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

															if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
															{
																FText::FromString("export_type : " + JsonObject->GetStringField("export_type"));

																	if (JsonObject->GetStringField("export_type") == "MaterialInstanceConstant")
																	{ 
																		// Log .mat file path for Material Asset creation in the future
																		GLog->Log(FText::FromString("File Path : " + JsonFilePath.RightChop(data_dirLen)));
																		GLog->Log(FText::FromString(" "));
																		
																		TArray<TSharedPtr<FJsonValue>> objArray = JsonObject->GetArrayField("TextureParameterValues");
																		for (int j = 0; j < objArray.Num(); j++)
																		{
																			TSharedPtr<FJsonValue> value = objArray[j];
																			TSharedPtr<FJsonObject> json = value->AsObject();

																			TSharedPtr<FJsonObject> ParameterInfo = json->GetObjectField("ParameterInfo");
																			FString MapName = ParameterInfo->GetStringField("Name");

																			if (MapName == "Diffuse")
																			{
																				TArray<FString> ParameterValue;
																				json->TryGetStringArrayField("ParameterValue", ParameterValue);
																				FString MapPath = ParameterValue[1];

																				GLog->Log(FText::FromString("Name : " + MapName + " PATH : " + MapPath));
																			}
																			if (MapName == "MRA")
																			{
																				TArray<FString> ParameterValue;
																				json->TryGetStringArrayField("ParameterValue", ParameterValue);
																				FString MapPath = ParameterValue[1];

																				GLog->Log(FText::FromString("Name : " + MapName + " PATH : " + MapPath));
																			}
																			if (MapName == "Normal")
																			{
																				TArray<FString> ParameterValue;
																				json->TryGetStringArrayField("ParameterValue", ParameterValue);
																				FString MapPath = ParameterValue[1];

																				GLog->Log(FText::FromString("Name : " + MapName + " PATH : " + MapPath));
																			}

																			//	MASTER v3 Path :	uMat_plugin/Plugins/uMat/Content/EnvBaseMat/BaseEnv_Blend_MAT_V4_V3Compatibility.uasset
																		}
																	}

																

																/*FString Name = "M_Material";
																FString PackageName = "/Game/";*/

																//UPackage* Package = CreatePackage(NULL, *PackageName);

																/*NewObject<UMaterialInstanceConstantFactoryNew>(NULL, PackageName+"Environment/"+Name, RF_NoFlags, nullptr, false, nullptr);*/

																/*FString PackageName = TEXT("/Game/SomePackageLocation/NewAssetName");
																UPackage* Package = CreatePackage(NULL, *PackageName);*/

															}
															else
															{
																//GLog->Log("ERROR, couldn't deserialize			pizdec");
															}


															//GLog->Log(FText::FromString("id " + FString::FromInt(i) + "MatFiles found = " + Files[i]) );
														}

														GLog->Log("uMat PLUGIN was STOPPED");
														return FReply::Handled(); })
					.Content()
					[
						SNew(STextBlock)
						.Text(FText::FromString("Run"))
					]
				]
		];
}

void FuMatModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(uMatTabName);
}

void FuMatModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FuMatCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FuMatCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FuMatModule, uMat)