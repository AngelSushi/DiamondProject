#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MecanismRewardActor.h"
#include "Door.generated.h"

UCLASS()
class DIAMONDPROJECT_API ADoor : public AMecanismRewardActor {
	GENERATED_BODY()
	
public:	
	ADoor();

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UDoorDataAsset> DoorAsset;

	UFUNCTION(BlueprintPure)
	UDoorDataAsset* GetDoorAsset() { return DoorAsset; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
