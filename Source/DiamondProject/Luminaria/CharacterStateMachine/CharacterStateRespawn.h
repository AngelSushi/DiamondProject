#pragma once

#include "CoreMinimal.h"
#include "CharacterState.h"
#include "CharacterStateRespawn.generated.h"


UCLASS()
class DIAMONDPROJECT_API UCharacterStateRespawn : public UCharacterState {
	GENERATED_BODY()

protected:
	virtual void OnStateBegin() override;
	virtual void OnStateTick(float DeltaTime) override;

private:
	UPROPERTY()
	float RespawnTimer = 1.5F;

	UPROPERTY()
	float RespawnTickTimer;
	
};
