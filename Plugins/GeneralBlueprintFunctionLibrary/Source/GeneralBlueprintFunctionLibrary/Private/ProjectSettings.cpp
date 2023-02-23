#include "ProjectSettings.h"

FString UProjectSettings::GetProjectVersion ()
{
    FString defaultGameIniPath = FString::Printf (TEXT ("%sDefaultGame.ini"), *FPaths::SourceConfigDir ());
    if (FPlatformFileManager::Get ().GetPlatformFile ().IsReadOnly (*defaultGameIniPath)) FPlatformFileManager::Get ().GetPlatformFile ().SetReadOnly (*defaultGameIniPath, false);
    FString ver;
    GConfig->GetString (TEXT ("/Script/EngineSettings.GeneralProjectSettings"), TEXT ("ProjectVersion"), ver, defaultGameIniPath);
    return ver;
}