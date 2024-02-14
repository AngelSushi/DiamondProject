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
	TObjectPtr<class ADiamondProjectCharacter> Leader;

	UPROPERTY()
	TArray<ADiamondProjectCharacter*> Characters;
	
	UFUNCTION()
	void RegisterPlayer(ADiamondProjectCharacter* Character);

	UFUNCTION()
	void OnPlayerMove(ADiamondProjectCharacter* player,FVector2D direction, bool& isCancelled);
	
};
