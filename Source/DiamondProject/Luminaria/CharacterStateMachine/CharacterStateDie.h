#pragma once

#include "CoreMinimal.h"
#include "CharacterState.h"
#include "CharacterStateDie.generated.h"


UCLASS()
class DIAMONDPROJECT_API UCharacterStateDie : public UCharacterState {
	GENERATED_BODY()

protected:
	virtual void OnStateInit() override;

private:
	UPROPERTY()
	TObjectPtr<class UPlayerManager> PlayerManager;

	UFUNCTION()
	void OnPlayerRespawn(ADiamondProjectCharacter* Character,EDeathCause DeathCause,FVector RespawnPoint);

};
