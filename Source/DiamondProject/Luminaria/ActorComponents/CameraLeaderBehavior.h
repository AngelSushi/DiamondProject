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
	TObjectPtr<class ACharacter> Leader;

	UPROPERTY()
	TArray<ACharacter*> Characters;
	
	UFUNCTION()
	void RegisterPlayer(ACharacter* Character);

	UFUNCTION()
	void OnPlayerMove(ACharacter* player,FVector2D direction, bool& isCancelled);
	
};
