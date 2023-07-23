#pragma once

#include "IVersioning.h"
#include "GitUtility.h"

class AUTOVERSIONING_API Versioning : public IVersioning
{
public:
	void SetPreReleaseText(string text);
	void SetBuildText(string text);
	Versioning(GitUtility* gitUtility);
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

	string RemoveIllegalChars(string text);
	string ApplyVersionIncrements(string ver);
};
