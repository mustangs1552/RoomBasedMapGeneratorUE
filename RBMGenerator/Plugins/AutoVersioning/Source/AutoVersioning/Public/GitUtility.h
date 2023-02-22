#pragma once

#include <string>
#include "CMDLineUtility.h"

using namespace std;

class AUTOVERSIONING_API GitUtility
{
public:
	const string gitLoc;
	const string repoLoc;

	GitUtility(string gitLocation, string repoLocation);
	string GetGitRepoVersion();
protected:
	CMDLineUtility* cmdLineUtility;
};
