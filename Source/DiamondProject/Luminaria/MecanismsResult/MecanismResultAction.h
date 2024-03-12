#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MecanismResultAction.generated.h"

UCLASS(ClassGroup = (Custom), Blueprintable, meta = (BlueprintSpawnableComponent))
class DIAMONDPROJECT_API UMecanismResultAction : public USceneComponent {

	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void DoAction();
};
