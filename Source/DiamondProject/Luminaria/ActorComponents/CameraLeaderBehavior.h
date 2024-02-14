#pragma once

#include "CoreMinimal.h"
#include "CameraBehavior.h"
#include "CameraLeaderBehavior.generated.h"

UCLASS()
class DIAMONDPROJECT_API UCameraLeaderBehavior : public UCameraBehavior
{
	GENERATED_BODY()

	UCameraLeaderBehavior();
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	ADiamondProjectCharacter* _leader;

	UPROPERTY()
	TArray<ADiamondProjectCharacter*> _characters;
	
	UFUNCTION()
	void RegisterPlayer(ADiamondProjectCharacter* Character);
	
	UFUNCTION()
	void OnPlayerDeath(ADiamondProjectCharacter* deathPlayer);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
