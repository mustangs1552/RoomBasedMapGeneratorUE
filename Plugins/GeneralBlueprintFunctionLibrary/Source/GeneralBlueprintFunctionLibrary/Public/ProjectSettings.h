#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ProjectSettings.generated.h"

/**
 * 
 */
UCLASS()
class GENERALBLUEPRINTFUNCTIONLIBRARY_API UProjectSettings : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	public:
		UFUNCTION (BlueprintPure)
		static FString GetProjectVersion ();
};
