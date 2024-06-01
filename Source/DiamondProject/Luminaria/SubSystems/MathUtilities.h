#pragma once

#include "CoreMinimal.h"
#include "Subsystems/Subsystem.h"
#include "MathUtilities.generated.h"

UCLASS()
class DIAMONDPROJECT_API UMathUtilities : public USubsystem {
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
	static float PingPongFloat(float Value, float Min, float Max);

	UFUNCTION(BlueprintPure)
	static float PingPongFloatAscending(float Value, float Min, float Max,bool& IsPing);

	
	UFUNCTION(BlueprintPure)
	static FVector PingPongVec(float Value, FVector Min, FVector Max);

	UFUNCTION(BlueprintPure)
	static FVector PingPongVecAscending(float Value, FVector Min, FVector Max,bool& IsPing);
};
