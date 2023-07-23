#include "GitUtility.h"

#include <algorithm>

GitUtility::GitUtility()
{
    cmdLineUtility = new CMDLineUtility();

	gitLoc = FindGitPath();
	repoLoc = FindRepoPath();
}

string GitUtility::GetGitRepoVersion()
{
    string cdCMD = "cd \"" + repoLoc + "\"";
    string gitCMD = "\"" + gitLoc + "\" describe --tags --long --match ""v[0-9]*""";
    string result = cmdLineUtility->ExecCMD(cdCMD + " & " + gitCMD);

    if (result.empty()) return "";
    replace(result.begin(), result.end(), '-', '.');
    return result.substr(1, result.find_last_of('.') - 1);
}

string GitUtility::FindGitPath()
{
	FString configPath = FPaths::GeneratedConfigDir();
	FString sourceControlIniPath;
	FConfigCacheIni::LoadGlobalIniFile(sourceControlIniPath, TEXT("SourceControlSettings"), nullptr, false, false, true, true, *configPath);
	FString gitPath;
	GConfig->GetString(TEXT("GitSourceControl.GitSourceControlSettings"), TEXT("BinaryPath"), gitPath, sourceControlIniPath);

	return TCHAR_TO_UTF8(*gitPath);
}

string GitUtility::FindRepoPath()
{
	FString projDir = FPaths::ProjectDir();
	return TCHAR_TO_UTF8(*projDir);
}
