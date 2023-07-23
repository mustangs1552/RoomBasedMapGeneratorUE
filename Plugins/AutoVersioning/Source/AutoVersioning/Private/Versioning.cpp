#include "Versioning.h"

Versioning::Versioning(GitUtility* gitUtility)
{
	git = gitUtility;
	majorVerIncrements = 0;
	minorVerIncrements = 0;
}

void Versioning::SetPreReleaseText(string text)
{
	preReleaseText = RemoveIllegalChars(text);
}
void Versioning::SetBuildText(string text)
{
	buildText = RemoveIllegalChars(text);
}

string Versioning::GitVersion()
{
	return git->GetGitRepoVersion();
}
string Versioning::Version()
{
	return ApplyVersionIncrements(GitVersion());
}
string Versioning::VersionPreRelease()
{
	string ver = Version();
	if (!ver.empty() && !preReleaseText.empty()) ver += "-" + preReleaseText;
	return ver;
}
string Versioning::VersionBuild()
{
	string ver = Version();
	if (!ver.empty() && !buildText.empty()) ver += "+" + buildText;
	return ver;
}
string Versioning::VersionPreReleaseBuild()
{
	string ver = VersionPreRelease();
	if (!ver.empty() && !buildText.empty()) ver += "+" + buildText;
	return ver;
}

void Versioning::IncrementMajorVersion()
{
	majorVerIncrements++;
}
void Versioning::IncrementMinorVersion()
{
	minorVerIncrements++;
}
void Versioning::RemoveIncrements()
{
	majorVerIncrements = 0;
	minorVerIncrements = 0;
}

FString Versioning::GetProjectSettingsVersion()
{
	FString defaultGameIniPath = FConfigCacheIni::NormalizeConfigIniPath(FString::Printf(TEXT("%sDefaultGame.ini"), *FPaths::SourceConfigDir()));
	if (FPlatformFileManager::Get().GetPlatformFile().IsReadOnly(*defaultGameIniPath)) FPlatformFileManager::Get().GetPlatformFile().SetReadOnly(*defaultGameIniPath, false);
	FString ver;
	GConfig->GetString(TEXT("/Script/EngineSettings.GeneralProjectSettings"), TEXT("ProjectVersion"), ver, defaultGameIniPath);
	return ver;
}
void Versioning::SetProjectSettingsVersion(string newVersion)
{
	if (!newVersion.empty())
	{
		FString defaultGameIniPath = FConfigCacheIni::NormalizeConfigIniPath(FString::Printf(TEXT("%sDefaultGame.ini"), *FPaths::SourceConfigDir()));
		if (FPlatformFileManager::Get().GetPlatformFile().IsReadOnly(*defaultGameIniPath)) FPlatformFileManager::Get().GetPlatformFile().SetReadOnly(*defaultGameIniPath, false);
		GConfig->SetString(TEXT("/Script/EngineSettings.GeneralProjectSettings"), TEXT("ProjectVersion"), UTF8_TO_TCHAR(newVersion.c_str()), defaultGameIniPath);
		GConfig->Flush(false, defaultGameIniPath);
	}
}

string Versioning::RemoveIllegalChars(string text)
{
	string validText;
	for (int i = 0; i < text.length(); i++)
	{
		if (text[i] == '.' || text[i] == '-' || (text[i] >= '0' && text[i] <= '9') || (text[i] >= 'A' && text[i] <= 'Z') || (text[i] >= 'a' && text[i] <= 'z'))
		{
			if (text[i] == '.' && i - 1 >= 0)
			{
				if (text[i - 1] != '.') validText += text[i];
			}
			else validText += text[i];
		}
	}
	return validText;
}

string Versioning::ApplyVersionIncrements(string ver)
{
	if (ver.empty()) return ver;

	string sections[3] = { "", "", "" };
	int currSectionI = 0;
	for (int i = 0; i < ver.length(); i++)
	{
		if (ver[i] == '.') currSectionI++;
		else sections[currSectionI] += ver[i];
	}

	int majorVer = stoi(sections[0]);
	int minorVer = stoi(sections[1]);
	int patchVer = stoi(sections[2]);
	if (majorVerIncrements > 0)
	{
		majorVer += majorVerIncrements;
		minorVer = 0;
		patchVer = 0;
	}
	else if (minorVerIncrements > 0)
	{
		minorVer += minorVerIncrements;
		patchVer = 0;
	}
	else patchVer += 1;

	return to_string(majorVer) + '.' + to_string(minorVer) + '.' + to_string(patchVer);
}