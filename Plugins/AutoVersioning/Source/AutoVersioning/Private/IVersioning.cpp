#include "IVersioning.h"

IVersioning::~IVersioning()
{

}

string IVersioning::GitVersion()
{
	return "";
}
string IVersioning::VersionPreReleaseBuild()
{
	return "";
}

void IVersioning::IncrementMajorVersion()
{

}
void IVersioning::IncrementMinorVersion()
{

}
void IVersioning::RemoveIncrements()
{

}
void IVersioning::SetPreReleaseText(string text)
{

}
void IVersioning::SetBuildText(string text)
{

}

void IVersioning::SetProjectSettingsVersion(string version)
{

}
void IVersioning::ResetProjectSettingsVersion()
{

}