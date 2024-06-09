#pragma once

#include "CoreMinimal.h"
#include "CharacterState.h"
#include "CharacterStateDie.generated.h"

class ADiamondProjectCharacter;
enum EDeathCause;

UCLASS()
class DIAMONDPROJECT_API UCharacterStateDie : public UCharacterState {
	GENERATED_BODY()

protected:
	virtual void OnStateInit() override;
	virtual void OnStateBegin() override;
	virtual void OnStateTick(float DeltaTime) override;

private:
	UPROPERTY()
	TObjectPtr<class UPlayerManager> PlayerManager;

	UFUNCTION()
	void OnPlayerRespawn(ADiamondProjectCharacter* Character,EDeathCause DeathCause,FVector RespawnPoint);

	UPROPERTY()
	float DieTimer = 3.F;

	UPROPERTY()
	float DieTickTimer;

	UPROPERTY()
	TObjectPtr<class ALink> LinkRef;
};
