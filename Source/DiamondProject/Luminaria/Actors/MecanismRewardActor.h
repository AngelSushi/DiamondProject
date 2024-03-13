#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MecanismRewardActor.generated.h"

UCLASS()
class DIAMONDPROJECT_API AMecanismRewardActor : public AActor {
	GENERATED_BODY()
	
public:	
	AMecanismRewardActor();

	virtual void Reward();
	virtual void CancelReward();

protected:
	virtual void BeginPlay() override;


	UFUNCTION(BlueprintImplementableEvent)
	void RewardFunction();

	UFUNCTION(BlueprintImplementableEvent)
	void CancelRewardFunction();


};
