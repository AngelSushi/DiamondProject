#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UIDataAsset.generated.h"


UCLASS()
class DIAMONDPROJECT_API UUIDataAsset : public UDataAsset {
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUIComboInput> ComboInputClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUIAbsorberInput> AbsorberInputClass;
	
};
