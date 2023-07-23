#pragma once

#include <string>
#include "CMDLineUtility.h"

using namespace std;

class AUTOVERSIONING_API GitUtility
{
public:
	string gitLoc;
	string repoLoc;

	GitUtility();
	string GetGitRepoVersion();
	string FindGitPath();
	string FindRepoPath();
protected:
	CMDLineUtility* cmdLineUtility;
};
