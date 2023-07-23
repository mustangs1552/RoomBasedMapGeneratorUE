#pragma once

#include <vector>
#include <string>
#include <map>
#include "CMDLineUtility.h"
#include "GitUtility.h"

using namespace std;

enum GitCMDArgTypes
{
	IncrementMajor,
	IncrementMinor,
	IncrementPatch,
	PreCommitMode,
	ApplyTag,
	ApplyTagPush,
	MergeUEVersion,
	ApplyUEProjectVersion
};

class AUTOVERSIONING_API VersioningToolUtility
{
public:
	map<GitCMDArgTypes, string> gitCMDArgTypeNames =
	{
		{ GitCMDArgTypes::IncrementMajor, "IncrementMajor" },
		{ GitCMDArgTypes::IncrementMinor, "IncrementMinor" },
		{ GitCMDArgTypes::IncrementPatch, "IncrementPatch" },
		{ GitCMDArgTypes::PreCommitMode, "PreCommitMode" },
		{ GitCMDArgTypes::ApplyTag, "ApplyTag" },
		{ GitCMDArgTypes::ApplyTagPush, "ApplyTagPush" },
		{ GitCMDArgTypes::MergeUEVersion, "MergeUEVersion" },
		{ GitCMDArgTypes::ApplyUEProjectVersion, "ApplyUEProjectVersion" }
	};

	VersioningToolUtility(GitUtility* gitUtility);
	string GitCMD(vector<GitCMDArgTypes> args, string preReleaseText = "", string buildText = "");
protected:
	CMDLineUtility* cmdLineUtility;
	GitUtility* git;
};
