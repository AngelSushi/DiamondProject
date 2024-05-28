#include "CharacterStateMachine.h"
#include "CharacterState.h"
#include "CharacterStateIdle.h"
#include "CharacterStateMovement.h"
#include "CharacterStateJump.h"
#include "CharacterStateDie.h"

#include "InputActionValue.h"
#include "../Core/DiamondProjectCharacter.h"

UCharacterStateMachine::UCharacterStateMachine() {
	StateIdle = CreateDefaultSubobject<UCharacterStateIdle>(TEXT("StateIdle"));
	StateMovement = CreateDefaultSubobject<UCharacterStateMovement>(TEXT("StateMovement"));
	StateJump = CreateDefaultSubobject<UCharacterStateJump>(TEXT("StateJump"));
	StateDie = CreateDefaultSubobject<UCharacterStateDie>(TEXT("StateDie"));
}


void UCharacterStateMachine::SMInit(ADiamondProjectCharacter* StateCharacter) {
	Character = StateCharacter;

	StateIdle->StateInit(this);
	StateMovement->StateInit(this);
	StateJump->StateInit(this);
	StateDie->StateInit(this);
}

void UCharacterStateMachine::SMBegin() {
	ChangeState(StateIdle);
}

void UCharacterStateMachine::SMTick(float DeltaTime) {
	if (CurrentState) {
		CurrentState->StateTick(DeltaTime);
	}
}

void UCharacterStateMachine::ChangeState(UCharacterState* NewState) {
	if (CurrentState != nullptr) {
		CurrentState->StateExit();
	}

	CurrentState = NewState;
	CharacterState = NewState->State;

	if (CurrentState) {
		GEngine->AddOnScreenDebugMessage(-1, 5.F, FColor::Cyan, FString::Printf(TEXT("[CharacterStateMachine - %s] NewState : %s"),*GetCharacter()->GetActorNameOrLabel(), *CurrentState->GetName()));
		CurrentState->StateBegin();
	}
}

void UCharacterStateMachine::OnMovement(const FInputActionValue& Value) {
	if (!CurrentState) {
		return;
	}

	CurrentState->OnMovement(Value);
}

void UCharacterStateMachine::OnJump() {
	if (!CurrentState) {
		return;
	}

	CurrentState->OnJump();
}

void UCharacterStateMachine::OnInputJumpReleased() {
	if (!CurrentState) {
		return;
	}

	CurrentState->OnInputJumpReleased();
}

void UCharacterStateMachine::OnDie() {
	if (!CurrentState) {
		return;
	}

	CurrentState->OnDie();
}
