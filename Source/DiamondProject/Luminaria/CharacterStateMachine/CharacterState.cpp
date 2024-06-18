#include "CharacterState.h"
#include "CharacterStateMachine.h"
#include "InputActionValue.h"
#include "../Actors/Absorber.h"

#include "../Core/DiamondProjectCharacter.h"
#include "../SubSystems/PlayerManager.h"

void UCharacterState::StateInit(UCharacterStateMachine* CharacterStateMachine) {
	StateMachine = CharacterStateMachine;
	PlayerManager = GetCharacter()->GetWorld()->GetSubsystem<UPlayerManager>();

	OnStateInit();
}

void UCharacterState::StateBegin() {
	OnStateBegin();
}

void UCharacterState::StateTick(float DeltaTime) {
	OnStateTick(DeltaTime);

	if (GetCharacter()->GetActorLocation() != LastPosition) {
		PlayerManager->OnPlayerMove.Broadcast(GetCharacter());
	}

	LastPosition = GetCharacter()->GetActorLocation();
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
void UCharacterState::OnMovement(const FInputActionValue& MovementValue) {}
void UCharacterState::OnJump() {}
void UCharacterState::OnInputJumpReleased() {}
void UCharacterState::OnDie() {}
void UCharacterState::OnAbsorberDetectCharacter(ADiamondProjectCharacter* Character, AAbsorber* Absorber) {}
void UCharacterState::OnAbsorberInputStarted(FKey Key) {}


	