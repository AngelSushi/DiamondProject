#include "CharacterStateMachine.h"
#include "CharacterState.h"
#include "CharacterStateIdle.h"
#include "CharacterStateMovement.h"
#include "CharacterStateJump.h"
#include "CharacterStateDie.h"
#include "CharacterStateFall.h"
#include "CharacterStateAttract.h"
#include "CharacterStateRespawn.h"
#include "CharacterStateReplacePosition.h"

#include "InputActionValue.h"
#include "../Core/DiamondProjectCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

UCharacterStateMachine::UCharacterStateMachine() {
	StateIdle = CreateDefaultSubobject<UCharacterStateIdle>(TEXT("StateIdle"));
	StateMovement = CreateDefaultSubobject<UCharacterStateMovement>(TEXT("StateMovement"));
	StateJump = CreateDefaultSubobject<UCharacterStateJump>(TEXT("StateJump"));
	StateDie = CreateDefaultSubobject<UCharacterStateDie>(TEXT("StateDie"));
	StateFall = CreateDefaultSubobject<UCharacterStateFall>(TEXT("StateFall"));
	StateAttract = CreateDefaultSubobject<UCharacterStateAttract>(TEXT("StateAttract"));
	StateRespawn = CreateDefaultSubobject<UCharacterStateRespawn>(TEXT("StateRespawn"));
	StateReplace = CreateDefaultSubobject<UCharacterStateReplacePosition>(TEXT("StateReplace"));
}


void UCharacterStateMachine::SMInit(ADiamondProjectCharacter* StateCharacter) {
	Character = StateCharacter;

	StateIdle->StateInit(this);
	StateMovement->StateInit(this);
	StateJump->StateInit(this);
	StateDie->StateInit(this);
	StateFall->StateInit(this);
	StateAttract->StateInit(this);
	StateRespawn->StateInit(this);
	StateReplace->StateInit(this);

	bForceReplacePosition = true;

}

void UCharacterStateMachine::SMBegin() {
	ChangeState(StateIdle);
}

void UCharacterStateMachine::SMTick(float DeltaTime) {
	if (CurrentState) {
		if (CurrentState != StateJump && CurrentState != StateFall) {
			GetCharacter()->GetCharacterMovement()->GravityScale = 5.0F;
		}
		
		CurrentState->StateTick(DeltaTime);
	}

	if (bForceReplacePosition) {
		if (!bLastForceReplacePosition) {
			StateReplace->StateBegin();
		}

		StateReplace->StateTick(DeltaTime);
	}

	bLastForceReplacePosition = bForceReplacePosition;
}

void UCharacterStateMachine::ChangeState(UCharacterState* NewState) {
	if (CurrentState != nullptr) {
		CurrentState->StateExit();
	}

	CurrentState = NewState;

	if (CurrentState) {
		CurrentState->StateBegin();
	}
}

void UCharacterStateMachine::OnMovement(const FInputActionValue& MovementValue) {
	if (!CurrentState) {
		return;
	}

	CurrentState->OnMovement(MovementValue);
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

void UCharacterStateMachine::OnAbsorberDetectCharacter(ADiamondProjectCharacter* DetectedCharacter, AAbsorber* Absorber) {
	if (!CurrentState) {
		return;
	}

	CurrentState->OnAbsorberDetectCharacter(DetectedCharacter, Absorber);
}

void UCharacterStateMachine::OnAbsorberInputStarted(FKey Key) {
	if (!CurrentState) {
		return;
	}

	CurrentState->OnAbsorberInputStarted(Key);
}
