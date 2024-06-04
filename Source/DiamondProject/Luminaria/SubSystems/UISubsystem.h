#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "../DataAssets/UIDataAsset.h"
#include "UISubsystem.generated.h"


UCLASS()
class DIAMONDPROJECT_API UUISubsystem : public UWorldSubsystem {
	GENERATED_BODY()

public:
	UUISubsystem();

	UUIDataAsset* GetUIAsset() { return UIAsset; }
private:
	UPROPERTY()
	TObjectPtr<class UUIDataAsset> UIAsset;
	
};
