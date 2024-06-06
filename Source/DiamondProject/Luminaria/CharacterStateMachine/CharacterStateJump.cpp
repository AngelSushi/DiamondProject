#include "CharacterStateJump.h"
#include "CharacterStateAttract.h"

#include "../Core/DiamondProjectPlayerController.h"
#include "../Core/DiamondProjectCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "../SubSystems/PlayerManager.h"
#include "CharacterStateIdle.h"
#include "CharacterStateDie.h"
#include "InputActionValue.h"


#include "../Interface/InputUI.h"
#include "../SubSystems/InputUIManager.h"
#include "../SubSystems/UISubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "../Actors/Link.h"

void UCharacterStateJump::OnStateInit() {
	Super::OnStateInit();

	LinkRef = Cast<ALink>(UGameplayStatics::GetActorOfClass(GetWorld(), ALink::StaticClass()));
}

void UCharacterStateJump::OnStateBegin() {
	Super::OnStateBegin();

	GetCharacter()->Jump();
	bIsJumping = true;
	bIsJumpPressed = true;

	//EnableInputListener();
	GetCharacter()->GetCharacterMovement()->GravityScale = 0.F;
	JumpTimer = 0.F;

	if (PlayerManager) {
		PlayerManager->OnPlayerLandOnGround.AddDynamic(this, &UCharacterStateJump::OnPlayerLandOnGround);

		if (PlayerManager->GetAllCharactersRef().Num() >= 2) {
			float DistanceBetweenPlayers = FVector::Distance(PlayerManager->GetAllCharactersRef()[0]->GetActorLocation(), PlayerManager->GetAllCharactersRef()[1]->GetActorLocation());

			if (LinkRef && DistanceBetweenPlayers >= LinkRef->GetDistanceMax()) {
				ChangeState(GetStateMachine()->StateIdle);
				return;
			}
		}
		
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
	GetCharacter()->GetCharacterMovement()->GravityScale = 5.F;
}

void UCharacterStateJump::OnDie() {
	ChangeState(GetStateMachine()->StateDie);
}

void UCharacterStateJump::OnAbsorberDetectCharacter(ADiamondProjectCharacter* Character, AAbsorber* Absorber) {
	ChangeState(GetStateMachine()->StateAttract);
}