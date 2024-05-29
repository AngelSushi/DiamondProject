#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UIInputDataAsset.generated.h"

enum EInput;

USTRUCT()
struct FInputData {

	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EInput> Input;

	UPROPERTY(EditAnywhere)
	FVector2D Offset;

	UPROPERTY(EditAnywhere)
	FVector2D Scale;
};

UCLASS()
class DIAMONDPROJECT_API UUIInputDataAsset : public UDataAsset {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FInputData> InputsIcon;
	
};
