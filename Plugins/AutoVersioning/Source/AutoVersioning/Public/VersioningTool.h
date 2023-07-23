#pragma once

#include "IVersioning.h"
#include "GitUtility.h"
#include "VersioningToolUtility.h"

class AUTOVERSIONING_API VersioningTool : public IVersioning
{
public:
	VersioningTool(GitUtility* gitUtility);

	virtual string GitVersion();
	virtual string VersionPreReleaseBuild();

	virtual void IncrementMajorVersion();
	virtual void IncrementMinorVersion();
	virtual void RemoveIncrements();
	virtual void SetPreReleaseText(string text);
	virtual void SetBuildText(string text);

	virtual void SetProjectSettingsVersion(string version);
	virtual void ResetProjectSettingsVersion();
protected:
	VersioningToolUtility* verToolUtility;
	bool incrementMajor = false;
	bool incrementMinor = false;
	string preReleaseText = "";
	string buildText = "";
};
