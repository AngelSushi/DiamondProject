#include "CharacterState.h"
#include "CharacterStateMachine.h"
#include "InputActionValue.h"
#include "../Actors/Absorber.h"

void UCharacterState::StateInit(UCharacterStateMachine* CharacterStateMachine) {
	StateMachine = CharacterStateMachine;

	OnStateInit();
}

void UCharacterState::StateBegin() {
	OnStateBegin();
}

void UCharacterState::StateTick(float DeltaTime) {
	OnStateTick(DeltaTime);
}

void UCharacterState::StateExit() {
	OnStateExit();
}

void UCharacterState::ChangeState(UCharacterState* NewState) const {
	StateMachine->ChangeState(NewState);
}

void UCharacterState::OnStateInit() {}
void UCharacterState::OnStateBegin() {}
void UCharacterState::OnStateTick(float DeltaTime) {}
void UCharacterState::OnStateExit() {}
void UCharacterState::OnMovement(const FInputActionValue& Value) {}
void UCharacterState::OnJump() {}
void UCharacterState::OnInputJumpReleased() {}
void UCharacterState::OnDie() {}
void UCharacterState::OnAbsorberDetectCharacter(ADiamondProjectCharacter* Character, AAbsorber* Absorber) {}


