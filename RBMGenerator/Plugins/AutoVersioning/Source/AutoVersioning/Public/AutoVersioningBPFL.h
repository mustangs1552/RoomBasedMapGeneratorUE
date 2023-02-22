#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AutoVersioningBPFL.generated.h"

/**
 * 
 */
UCLASS()
class AUTOVERSIONING_API UAutoVersioningBPFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	public:
		UFUNCTION(BlueprintCallable)
		static FString UpdateAndApplyVersion();
};
