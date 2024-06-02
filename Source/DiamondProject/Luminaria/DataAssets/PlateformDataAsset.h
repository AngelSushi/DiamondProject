#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlateformDataAsset.generated.h"


UCLASS()
class DIAMONDPROJECT_API UPlateformDataAsset : public UDataAsset {
	GENERATED_BODY()
	
public: 
	UPROPERTY(EditAnywhere)
	float Speed = 100.F;

	UPROPERTY(EditAnywhere)
	float Acceleration = 2.F;
};
