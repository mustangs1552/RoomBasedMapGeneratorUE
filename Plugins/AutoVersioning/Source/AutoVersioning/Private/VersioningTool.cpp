#include "VersioningTool.h"

VersioningTool::VersioningTool(GitUtility* gitUtility)
{
	verToolUtility = new VersioningToolUtility(gitUtility);
}

string VersioningTool::GitVersion()
{
	return verToolUtility->GitCMD({});
}
string VersioningTool::VersionPreReleaseBuild()
{
	vector<GitCMDArgTypes> args =
	{
		GitCMDArgTypes::MergeUEVersion
	};
	if (incrementMajor) args.push_back(GitCMDArgTypes::IncrementMajor);
	else if (incrementMinor) args.push_back(GitCMDArgTypes::IncrementMinor);

	return verToolUtility->GitCMD(args, preReleaseText, buildText);
}

void VersioningTool::IncrementMajorVersion()
{
	incrementMajor = true;
	incrementMinor = false;
}
void VersioningTool::IncrementMinorVersion()
{
	incrementMajor = false;
	incrementMinor = true;
}
void VersioningTool::RemoveIncrements()
{
	incrementMajor = false;
	incrementMinor = false;
}
void VersioningTool::SetPreReleaseText(string text)
{
	preReleaseText = text;
}
void VersioningTool::SetBuildText(string text)
{
	buildText = text;
}

void VersioningTool::SetProjectSettingsVersion(string version)
{
	vector<GitCMDArgTypes> args =
	{
		GitCMDArgTypes::MergeUEVersion,
		GitCMDArgTypes::ApplyUEProjectVersion
	};
	if (incrementMajor) args.push_back(GitCMDArgTypes::IncrementMajor);
	else if (incrementMinor) args.push_back(GitCMDArgTypes::IncrementMinor);

	verToolUtility->GitCMD(args, preReleaseText, buildText);
}
void VersioningTool::ResetProjectSettingsVersion()
{
	vector<GitCMDArgTypes> args =
	{
		GitCMDArgTypes::PreCommitMode,
		GitCMDArgTypes::ApplyUEProjectVersion
	};
	verToolUtility->GitCMD(args, preReleaseText, buildText);
}