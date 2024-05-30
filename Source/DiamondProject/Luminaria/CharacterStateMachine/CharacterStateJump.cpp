#include "CharacterStateJump.h"
#include "CharacterStateAttract.h"

#include "../Core/DiamondProjectPlayerController.h"
#include "../Core/DiamondProjectCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "../SubSystems/PlayerManager.h"
#include "CharacterStateIdle.h"
#include "CharacterStateDie.h"
#include <InputActionValue.h>

void UCharacterStateJump::OnStateBegin() {
	Super::OnStateBegin();

	GetCharacter()->Jump();
	bIsJumping = true;
	bIsJumpPressed = true;

	//GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Magenta, FString::Printf(TEXT("Jump From %s"), *GetActorNameOrLabel()));

	GetCharacter()->GetCharacterMovement()->GravityScale = 0.F;
	JumpTimer = 0.F;

	if (PlayerManager) {
		PlayerManager->OnPlayerLandOnGround.AddDynamic(this, &UCharacterStateJump::OnPlayerLandOnGround);
	}
}

void UCharacterStateJump::OnStateTick(float DeltaTime) {
	if (bIsJumping) {
		JumpTimer += DeltaTime;

		if (JumpTimer >= JumpMaxDuration) {
			StopJump();
		}

		if (!bIsJumpPressed && JumpTimer >= JumpMinDuration) {
			StopJump();
		}
	}
}

void UCharacterStateJump::OnInputJumpReleased() {
	if (JumpTimer > JumpMinDuration) {
		StopJump();
	}

	bIsJumpPressed = false;
}

void UCharacterStateJump::OnPlayerLandOnGround(ADiamondProjectCharacter* Character) {
	if (bIsJumping) {
		bIsJumping = false;
		ChangeState(GetStateMachine()->StateIdle);
	}
}

void UCharacterStateJump::StopJump() {
	GetCharacter()->GetCharacterMovement()->GravityScale = GetCharacter()->GetGravityScaleSaved();
}

void UCharacterStateJump::OnDie() {
	ChangeState(GetStateMachine()->StateDie);
}

void UCharacterStateJump::OnAbsorberDetectCharacter(ADiamondProjectCharacter* Character, AAbsorber* Absorber) {
	ChangeState(GetStateMachine()->StateAttract);
}
