#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerAsset.generated.h"


UCLASS()
class DIAMONDPROJECT_API UPlayerAsset : public UDataAsset {
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	float MinEnergy = 0.F;

	UPROPERTY(EditAnywhere)
	float MaxEnergy = 100000.F;

	UPROPERTY(EditAnywhere)
	float Speed = 600.F;

	UPROPERTY(EditAnywhere)
	float SpeedIncrease = 40.F;

	UPROPERTY(EditAnywhere)
	float JumpMinDuration = 0.05F;

	UPROPERTY(EditAnywhere)
	float JumpMaxDuration = 0.35F;

	UPROPERTY(EditAnywhere)
	float JumpDurationIncrease = 0.02F;
};
