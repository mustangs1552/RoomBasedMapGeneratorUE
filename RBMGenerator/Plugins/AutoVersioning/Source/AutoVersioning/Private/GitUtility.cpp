#include "GitUtility.h"

#include <algorithm>

GitUtility::GitUtility(string gitLocation, string repoLocation) : gitLoc(gitLocation), repoLoc(repoLocation)
{
    cmdLineUtility = new CMDLineUtility();
}

string GitUtility::GetGitRepoVersion()
{
    string cdCMD = "cd \"" + repoLoc + "\"";
    string gitCMD = "\"" + gitLoc + "\" describe --tags --long --match ""v[0-9]*""";
    string result = cmdLineUtility->ExecCMD(cdCMD + " & " + gitCMD);

    if (result.empty()) return "";
    replace(result.begin(), result.end(), '-', '.');
    return result.substr(1, result.find_last_of('.') - 1);
}