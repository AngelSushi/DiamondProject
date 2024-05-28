#include "CharacterStateIdle.h"
#include "CharacterStateMovement.h"
#include "CharacterStateJump.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"

void UCharacterStateIdle::OnStateBegin() {}

void UCharacterStateIdle::OnStateTick(float DeltaTime) {}

void UCharacterStateIdle::OnMovement(const FInputActionValue& Value) {
	ChangeState(GetStateMachine()->StateMovement);
}

void UCharacterStateIdle::OnJump() {
	ChangeState(GetStateMachine()->StateJump);
}

void UCharacterStateIdle::OnDie() {
	ChangeState(GetStateMachine()->StateDie);
}
