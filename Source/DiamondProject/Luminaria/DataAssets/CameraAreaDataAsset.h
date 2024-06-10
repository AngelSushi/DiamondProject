#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../../../DiamondProject/Luminaria/Actors/LuminariaCamera.h"
#include "LuminariaDataAsset.h"
#include "CameraAreaDataAsset.generated.h"


UCLASS()
class DIAMONDPROJECT_API UCameraAreaDataAsset : public ULuminariaDataAsset {
	GENERATED_BODY()

public: 
	UPROPERTY(EditAnywhere)
	int16 Id = 0;

	UPROPERTY(EditAnywhere)
	uint8 PlayerNeeded = 2;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECameraBehavior> AreaBehavior;

	UPROPERTY(EditAnywhere)
	uint16 ZoomMin;

	UPROPERTY(EditAnywhere)
	uint16 ZoomMax;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float HeightMin;

	UPROPERTY(EditAnywhere)
	float HeightMax;

	UPROPERTY(EditAnywhere)
	float PlayerSpeedMin = 600.F;

	UPROPERTY(EditAnywhere)
	float PlayerSpeedMax = 600.F;

	UPROPERTY(EditAnywhere)
	float TransitionDuration = 1200.F;

	UPROPERTY(EditAnywhere)
	float ZoomDuration = 8.F;
	
};
