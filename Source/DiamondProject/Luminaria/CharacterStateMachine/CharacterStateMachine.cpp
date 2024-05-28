#include "DiamondProject/Luminaria/CharacterStateMachine/CharacterStateMachine.h"
#include "DiamondProject/Luminaria/CharacterStateMachine/CharacterState.h"

void UCharacterStateMachine::BeginStateMachine() {
}

void UCharacterStateMachine::StateTick(float DeltaTime) {
	if (CurrentState) {
		CurrentState->StateTick(DeltaTime);
	}
}

void UCharacterStateMachine::SwitchState(UCharacterState* NewState) {
	if (CurrentState != nullptr) {
		CurrentState->StateExit();
	}

	CurrentState = NewState;
	CharacterState = NewState->State;

	if (CurrentState) {
		CurrentState->StateBegin();
	}
}
