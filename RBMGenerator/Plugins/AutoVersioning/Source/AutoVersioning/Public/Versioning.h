#pragma once

#include "GitUtility.h"

class AUTOVERSIONING_API Versioning
{
public:
	void SetPreReleaseText(string text);
	void SetBuildText(string text);
	string GetGitPath();
	string GetRepoPath();
	Versioning();
	string GitVersion();
	string Version();
	string VersionPreRelease();
	string VersionBuild();
	string VersionPreReleaseBuild();
	void IncrementMajorVersion();
	void IncrementMinorVersion();
	void RemoveIncrements();
	FString GetProjectSettingsVersion();
	void SetProjectSettingsVersion(string newVersion);
protected:
	GitUtility* git;
	string preReleaseText;
	string buildText;
	int majorVerIncrements;
	int minorVerIncrements;

	FString FindGitPath();
	FString FindRepoPath();
	string RemoveIllegalChars(string text);
	string ApplyVersionIncrements(string ver);
};
