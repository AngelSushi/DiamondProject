#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MecanismResultAction.generated.h"

UCLASS(BlueprintType,Blueprintable)
class DIAMONDPROJECT_API UMecanismResultAction : public UObject {

	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void OnMecanismTurnOn();
};
