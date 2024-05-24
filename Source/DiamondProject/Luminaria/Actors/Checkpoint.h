#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Checkpoint.generated.h"


UCLASS()
class DIAMONDPROJECT_API ACheckpoint : public ATriggerBox
{
	GENERATED_BODY()
	
public: 
	ACheckpoint();

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> checkPoint;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBillboardComponent> Billboard;
};
