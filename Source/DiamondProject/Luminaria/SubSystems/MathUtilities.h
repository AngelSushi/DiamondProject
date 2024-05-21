#pragma once

#include "CoreMinimal.h"
#include "Subsystems/Subsystem.h"
#include "MathUtilities.generated.h"

UCLASS()
class DIAMONDPROJECT_API UMathUtilities : public USubsystem {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,BlueprintPure)
	static float PingPong(float Value, float Min, float Max);
	
};
