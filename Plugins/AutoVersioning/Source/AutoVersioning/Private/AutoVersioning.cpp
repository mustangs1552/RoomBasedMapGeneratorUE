#include "AutoVersioning.h"
#include "AutoVersioningStyle.h"
#include "AutoVersioningCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "VersioningTool.h"

static const FName AutoVersioningTabName("Auto Versioning");

#define LOCTEXT_NAMESPACE "FAutoVersioningModule"

void FAutoVersioningModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FAutoVersioningStyle::Initialize();
	FAutoVersioningStyle::ReloadTextures();

	FAutoVersioningCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FAutoVersioningCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FAutoVersioningModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FAutoVersioningModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(AutoVersioningTabName, FOnSpawnTab::CreateRaw(this, &FAutoVersioningModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FAutoVersioningTabTitle", "Auto Versioning"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	gitUtility = new GitUtility();
}

void FAutoVersioningModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FAutoVersioningStyle::Shutdown();

	FAutoVersioningCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(AutoVersioningTabName);
}

TSharedRef<SDockTab> FAutoVersioningModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	string gitPath = gitUtility->repoLoc;
	string repoPath = gitUtility->gitLoc;

	return SNew(SDockTab).TabRole(ETabRole::NomadTab).ShouldAutosize(true)
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center).AutoHeight().Padding(5, 10, 5, 5)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot().VAlign(VAlign_Center).AutoWidth()
			[
				SNew(STextBlock).Text(FText::FromString("Generated Version: "))
			]
			+SHorizontalBox::Slot().VAlign(VAlign_Center).AutoWidth()
			[
				SNew(SEditableText).IsReadOnly(true).Text_Raw(this, &FAutoVersioningModule::GetVersionText)
			]
		]
		+SVerticalBox::Slot().HAlign(HAlign_Center).VAlign(VAlign_Top).AutoHeight().Padding(5)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot().AutoWidth()
			[
				SNew(SButton).Text(FText::FromString("Increment Major Version")).OnClicked_Raw(this, &FAutoVersioningModule::OnIncrementMajorVersionButtonClicked)
			]
			+SHorizontalBox::Slot().AutoWidth()
			[
				SNew(SButton).Text(FText::FromString("Increment Minor Version")).OnClicked_Raw(this, &FAutoVersioningModule::OnIncrementMinorVersionButtonClicked)
			]
			+SHorizontalBox::Slot().AutoWidth()
			[
				SNew(SButton).Text(FText::FromString("Remove Increments")).OnClicked_Raw(this, &FAutoVersioningModule::OnRemoveIncrementsButtonClicked)
			]
		]
		+SVerticalBox::Slot().HAlign(HAlign_Center).VAlign(VAlign_Top).AutoHeight().Padding(5, 20, 5, 5)
		[
			SNew(STextBlock).Text(FText::FromString("Settings"))
		]
		+SVerticalBox::Slot().HAlign(HAlign_Center).VAlign(VAlign_Top).AutoHeight().Padding(5)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot().AutoWidth()
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot().HAlign(HAlign_Right).Padding(5)
				[
					SNew(STextBlock).Justification(ETextJustify::Right).Text(FText::FromString("Add Pre-Release Section"))
				]
				+SVerticalBox::Slot().HAlign(HAlign_Right).Padding(5)
				[
					SNew(STextBlock).Justification(ETextJustify::Right).Text(FText::FromString("Pre-Release Text"))
				]
				+SVerticalBox::Slot().HAlign(HAlign_Right).Padding(5)
				[
					SNew(STextBlock).Justification(ETextJustify::Right).Text(FText::FromString("Add Build Section"))
				]
				+SVerticalBox::Slot().HAlign(HAlign_Right).Padding(5)
				[
					SNew(STextBlock).Justification(ETextJustify::Right).Text(FText::FromString("Build Text"))
				]
			]
			+SHorizontalBox::Slot().AutoWidth()
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot().HAlign(HAlign_Left).Padding(5)
				[
					SNew(SCheckBox).HAlign(HAlign_Left).IsChecked_Raw(this, &FAutoVersioningModule::GetUsePreReleaseText).OnCheckStateChanged_Raw(this, &FAutoVersioningModule::OnUsePreReleaseTextCBStateChanged)
				]
				+SVerticalBox::Slot().HAlign(HAlign_Left).Padding(5)
				[
					SNew(SEditableTextBox).MinDesiredWidth(150).Text_Raw(this, &FAutoVersioningModule::GetPreReleaseText).OnTextCommitted_Raw(this, &FAutoVersioningModule::OnPreReleaseTextETBTextCommitted)
				]
				+SVerticalBox::Slot().HAlign(HAlign_Left).Padding(5)
				[
					SNew(SCheckBox).HAlign(HAlign_Left).IsChecked_Raw(this, &FAutoVersioningModule::GetUseBuildText).OnCheckStateChanged_Raw(this, &FAutoVersioningModule::OnUseBuildTextCBStateChanged)
				]
				+SVerticalBox::Slot().HAlign(HAlign_Left).Padding(5)
				[
					SNew(SEditableTextBox).MinDesiredWidth(150).Text_Raw(this, &FAutoVersioningModule::GetBuildText).OnTextCommitted_Raw(this, &FAutoVersioningModule::OnBuildTextETBTextCommitted)
				]
			]
		]
		+SVerticalBox::Slot().HAlign(HAlign_Center).VAlign(VAlign_Top).AutoHeight().Padding(5, 10, 5, 5)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot().AutoWidth()
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot().HAlign(HAlign_Right).Padding(5)
				[
					SNew(STextBlock).Justification(ETextJustify::Right).Text(FText::FromString("Git Path:"))
				]
				+SVerticalBox::Slot().HAlign(HAlign_Right).Padding(5)
				[
					SNew(STextBlock).Justification(ETextJustify::Right).Text(FText::FromString("Repo Path:"))
				]
			]
			+SHorizontalBox::Slot().AutoWidth()
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot().HAlign(HAlign_Left).Padding(5)
				[
					SNew(STextBlock).Text(FText::FromString((gitPath.empty()) ? "Unable to get Git path." : gitPath.c_str()))
				]
				+SVerticalBox::Slot().HAlign(HAlign_Left).Padding(5)
				[
					SNew(STextBlock).Text(FText::FromString((repoPath.empty()) ? "Unable to get repo path." : repoPath.c_str()))
				]
			]
		]
		+SVerticalBox::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center).AutoHeight().Padding(5)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot().VAlign(VAlign_Center).AutoWidth()
			[
				SNew(STextBlock).Text(FText::FromString("Version from Git: "))
			]
			+SHorizontalBox::Slot().VAlign(VAlign_Center).AutoWidth()
			[
				SNew(SEditableText).IsReadOnly(true).Text_Raw(this, &FAutoVersioningModule::GetGitVersionText)
			]
			+SHorizontalBox::Slot().VAlign(VAlign_Center).AutoWidth().Padding(5, 0)
			[
				SNew(SButton).Text(FText::FromString("Update")).OnClicked_Raw(this, &FAutoVersioningModule::OnUpdateButtonClicked)
			]
		]
		+SVerticalBox::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center).AutoHeight().Padding(5)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot().VAlign(VAlign_Center).AutoWidth().Padding(5, 0)
			[
				SNew(SButton).Text(FText::FromString("Save Generated Version")).OnClicked_Raw(this, &FAutoVersioningModule::OnApplyVersionButtonClicked)
			]
			+SHorizontalBox::Slot().AutoWidth()
			[
				SNew(SButton).Text(FText::FromString("Reset Saved Version")).OnClicked_Raw(this, &FAutoVersioningModule::OnResetVersionButtonClicked)
			]
		]
	];
}

void FAutoVersioningModule::PluginButtonClicked()
{
	SetupVersioning();
	FGlobalTabmanager::Get()->TryInvokeTab(AutoVersioningTabName);
}

void FAutoVersioningModule::SaveSettings() const
{
	FString defaultGameIniPath = FConfigCacheIni::NormalizeConfigIniPath(FString::Printf(TEXT("%sDefaultGame.ini"), *FPaths::SourceConfigDir()));
	if (FPlatformFileManager::Get().GetPlatformFile().IsReadOnly(*defaultGameIniPath)) FPlatformFileManager::Get().GetPlatformFile().SetReadOnly(*defaultGameIniPath, false);

	GConfig->SetString(TEXT("/Script/AutoVersioningSettings"), TEXT("UsePreReleaseText"), UTF8_TO_TCHAR(((usePreReleaseText) ? "true" : "false")), defaultGameIniPath);
	GConfig->SetString(TEXT("/Script/AutoVersioningSettings"), TEXT("PreReleaseText"), UTF8_TO_TCHAR(preReleaseText.c_str()), defaultGameIniPath);
	GConfig->SetString(TEXT("/Script/AutoVersioningSettings"), TEXT("UseBuildText"), UTF8_TO_TCHAR(((useBuildText) ? "true" : "false")), defaultGameIniPath);
	GConfig->SetString(TEXT("/Script/AutoVersioningSettings"), TEXT("BuildText"), UTF8_TO_TCHAR(buildText.c_str()), defaultGameIniPath);
	
	GConfig->Flush(false, defaultGameIniPath);
}
void FAutoVersioningModule::LoadSettings()
{
	FString defaultGameIniPath = FConfigCacheIni::NormalizeConfigIniPath(FString::Printf(TEXT("%sDefaultGame.ini"), *FPaths::SourceConfigDir()));
	if (FPlatformFileManager::Get().GetPlatformFile().IsReadOnly(*defaultGameIniPath)) FPlatformFileManager::Get().GetPlatformFile().SetReadOnly(*defaultGameIniPath, false);

	FString loadedUsePreReleaseText;
	GConfig->GetString(TEXT("/Script/AutoVersioningSettings"), TEXT("UsePreReleaseText"), loadedUsePreReleaseText, defaultGameIniPath);
	FString loadedPreReleaseText;
	GConfig->GetString(TEXT("/Script/AutoVersioningSettings"), TEXT("PreReleaseText"), loadedPreReleaseText, defaultGameIniPath);
	FString loadedUseBuildText;
	GConfig->GetString(TEXT("/Script/AutoVersioningSettings"), TEXT("UseBuildText"), loadedUseBuildText, defaultGameIniPath);
	FString loadedBuildText;
	GConfig->GetString(TEXT("/Script/AutoVersioningSettings"), TEXT("BuildText"), loadedBuildText, defaultGameIniPath);

	usePreReleaseText = loadedUsePreReleaseText.ToLower() == "true";
	preReleaseText = TCHAR_TO_UTF8(*loadedPreReleaseText);
	useBuildText = loadedUseBuildText.ToLower() == "true";
	buildText = TCHAR_TO_UTF8(*loadedBuildText);
}

void FAutoVersioningModule::SetupVersioning()
{
	versioning = new VersioningTool(gitUtility);
	LoadSettings();
	UpdateVersion();
	gitVersion = versioning->GitVersion();
}

void FAutoVersioningModule::UpdateVersioning() const
{
	versioning->SetPreReleaseText((usePreReleaseText) ? preReleaseText : "");
	versioning->SetBuildText((useBuildText) ? buildText : "");
}
string FAutoVersioningModule::UpdateVersion() const
{
	UpdateVersioning();
	version = versioning->VersionPreReleaseBuild();
	return version;
}
string FAutoVersioningModule::UpdateAndApplyVersion()
{
	if (versioning == NULL) SetupVersioning();
	string ver = UpdateVersion();
	OnApplyVersionButtonClicked();
	return ver;
}

FText FAutoVersioningModule::GetGitVersionText() const
{
	return FText::FromString((gitVersion.empty()) ? "Unable to get version." : gitVersion.c_str());
}
FText FAutoVersioningModule::GetVersionText() const
{
	return FText::FromString((version.empty()) ? "Unable to get version." : version.c_str());
}
FReply FAutoVersioningModule::OnUpdateButtonClicked() const
{
	gitVersion = versioning->GitVersion();
	return FReply::Handled();
}
FReply FAutoVersioningModule::OnIncrementMajorVersionButtonClicked() const
{
	versioning->IncrementMajorVersion();
	UpdateVersion();
	return FReply::Handled();
}
FReply FAutoVersioningModule::OnIncrementMinorVersionButtonClicked() const
{
	versioning->IncrementMinorVersion();
	UpdateVersion();
	return FReply::Handled();
}
FReply FAutoVersioningModule::OnRemoveIncrementsButtonClicked() const
{
	versioning->RemoveIncrements();
	UpdateVersion();
	return FReply::Handled();
}
FReply FAutoVersioningModule::OnApplyVersionButtonClicked() const
{
	versioning->SetProjectSettingsVersion(version);
	return FReply::Handled();
}
FReply FAutoVersioningModule::OnResetVersionButtonClicked() const
{
	versioning->ResetProjectSettingsVersion();
	return FReply::Handled();
}

ECheckBoxState FAutoVersioningModule::GetUsePreReleaseText() const
{
	return (usePreReleaseText) ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}
void FAutoVersioningModule::OnUsePreReleaseTextCBStateChanged(ECheckBoxState inState) const
{
	switch (inState)
	{
		case ECheckBoxState::Checked:
			usePreReleaseText = true;
			break;
		default:
			usePreReleaseText = false;
			break;
	}
	UpdateVersion();
	SaveSettings();
}
FText FAutoVersioningModule::GetPreReleaseText() const
{
	return FText::FromString(UTF8_TO_TCHAR(preReleaseText.c_str()));
}
void FAutoVersioningModule::OnPreReleaseTextETBTextCommitted(const FText& newText, ETextCommit::Type commitType)
{
	preReleaseText = string(TCHAR_TO_UTF8(*newText.ToString()));
	UpdateVersion();
	SaveSettings();
}

ECheckBoxState FAutoVersioningModule::GetUseBuildText() const
{
	return (useBuildText) ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}
void FAutoVersioningModule::OnUseBuildTextCBStateChanged(ECheckBoxState inState) const
{
	switch (inState)
	{
		case ECheckBoxState::Checked:
			useBuildText = true;
			break;
		default:
			useBuildText = false;
			break;
	}
	UpdateVersion();
	SaveSettings();
}
FText FAutoVersioningModule::GetBuildText() const
{
	return FText::FromString(UTF8_TO_TCHAR(buildText.c_str()));
}
void FAutoVersioningModule::OnBuildTextETBTextCommitted(const FText& newText, ETextCommit::Type commitType)
{
	buildText = string(TCHAR_TO_UTF8(*newText.ToString()));
	UpdateVersion();
	SaveSettings();
}

void FAutoVersioningModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FAutoVersioningCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FAutoVersioningCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAutoVersioningModule, AutoVersioning)