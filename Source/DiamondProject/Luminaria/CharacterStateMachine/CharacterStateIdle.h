#pragma once

#include "CoreMinimal.h"
#include "CharacterState.h"
#include "CharacterStateIdle.generated.h"

UCLASS()
class DIAMONDPROJECT_API UCharacterStateIdle : public UCharacterState {
	GENERATED_BODY()
	
public:

	virtual void OnMovement(const FInputActionValue& Value) override;
	virtual void OnJump() override;
	virtual void OnDie() override;

private:
	virtual void OnStateBegin() override;
	virtual void OnStateTick(float DeltaTime) override;
};
