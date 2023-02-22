#include "AutoVersioningBPFL.h"
#include "AutoVersioning.h"

FString UAutoVersioningBPFL::UpdateAndApplyVersion()
{
	FAutoVersioningModule* autoVersioningModule = FModuleManager::GetModulePtr<FAutoVersioningModule>(FName("AutoVersioning"));
	if (autoVersioningModule == NULL) return "ERROR: Couldn't find a 'FAutoVersioningModule' by name 'AutoVersioning'!";
	return autoVersioningModule->UpdateAndApplyVersion().c_str();
}