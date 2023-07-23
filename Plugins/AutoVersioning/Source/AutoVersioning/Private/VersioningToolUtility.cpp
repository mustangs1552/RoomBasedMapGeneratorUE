#include "VersioningToolUtility.h"

VersioningToolUtility::VersioningToolUtility(GitUtility* gitUtility)
{
	git = gitUtility;
	cmdLineUtility = new CMDLineUtility();
}

string VersioningToolUtility::GitCMD(vector<GitCMDArgTypes> args, string preReleaseText, string buildText)
{
	string cdCMD = "cd \"" + git->repoLoc + "\"";
	string toolCMD = "\"Plugins\\AutoVersioning\\VersioningTool.exe\" Git \"RepoPath=" + git->repoLoc + "\" ";
	for (GitCMDArgTypes arg : args) toolCMD += gitCMDArgTypeNames[arg] + " ";
	toolCMD += '-' + preReleaseText + " " + '+' + buildText;

	FString cmdFString = UTF8_TO_TCHAR(toolCMD.c_str());
	UE_LOG(LogTemp, Log, TEXT("Auto Versioning: %s"), *cmdFString);
	return cmdLineUtility->ExecCMD(cdCMD + " & " + toolCMD);
}