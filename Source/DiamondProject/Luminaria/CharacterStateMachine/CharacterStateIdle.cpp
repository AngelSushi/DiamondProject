#include "CharacterStateIdle.h"
#include "CharacterStateMovement.h"
#include "CharacterStateDie.h"
#include "CharacterStateJump.h"
#include "CharacterStateAttract.h"
#include "CharacterStateFall.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCharacterStateIdle::OnStateBegin() {
	Super::OnStateBegin();
}

void UCharacterStateIdle::OnStateTick(float DeltaTime) {
	Super::OnStateTick(DeltaTime);

	if (!GetCharacter()->GetCharacterMovement()->IsMovingOnGround()) {
		//ChangeState(GetStateMachine()->StateFall);
	}


}

void UCharacterStateIdle::OnMovement(const FInputActionValue& MovementValue) {

	if (MovementValue.Get<FVector2D>().Length() < 0.2F) {
		return;
	}

	ChangeState(GetStateMachine()->StateMovement);
}

void UCharacterStateIdle::OnJump() {
	ChangeState(GetStateMachine()->StateJump);
}

void UCharacterStateIdle::OnDie() {
	ChangeState(GetStateMachine()->StateDie);
}

void UCharacterStateIdle::OnAbsorberDetectCharacter(ADiamondProjectCharacter* Character, AAbsorber* Absorber) {
	ChangeState(GetStateMachine()->StateAttract);
}
