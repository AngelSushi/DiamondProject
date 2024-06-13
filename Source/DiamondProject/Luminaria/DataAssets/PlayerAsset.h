#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LuminariaDataAsset.h"
#include "PlayerAsset.generated.h"


UCLASS()
class DIAMONDPROJECT_API UPlayerAsset : public ULuminariaDataAsset {
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

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float JumpMinDuration = 0.05F;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float JumpMaxDuration = 0.35F;

	UPROPERTY(EditAnywhere)
	float JumpDurationIncrease = 0.02F;
};
