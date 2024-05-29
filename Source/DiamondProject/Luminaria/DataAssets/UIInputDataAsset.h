#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UIInputDataAsset.generated.h"

enum EInput;

UCLASS()
class DIAMONDPROJECT_API UUIInputDataAsset : public UDataAsset {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TMap<TEnumAsByte<EInput>, UTexture2D*> InputsIcon;
	
};
