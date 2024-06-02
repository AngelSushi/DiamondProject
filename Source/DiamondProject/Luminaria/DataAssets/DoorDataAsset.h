#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LuminariaDataAsset.h"
#include "DoorDataAsset.generated.h"


UCLASS()
class DIAMONDPROJECT_API UDoorDataAsset : public ULuminariaDataAsset {
	GENERATED_BODY()

public: 
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float DoorTime = 2.F;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<EAxis::Type> DoorAxis;
	
};
