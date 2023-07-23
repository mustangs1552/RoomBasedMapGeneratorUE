#pragma once

#include <string>

using namespace std;

class AUTOVERSIONING_API IVersioning
{
public:
	virtual ~IVersioning();

	virtual string GitVersion();
	virtual string VersionPreReleaseBuild();

	virtual void IncrementMajorVersion();
	virtual void IncrementMinorVersion();
	virtual void RemoveIncrements();
	virtual void SetPreReleaseText(string text);
	virtual void SetBuildText(string text);

	virtual void SetProjectSettingsVersion(string version);
	virtual void ResetProjectSettingsVersion();
};
