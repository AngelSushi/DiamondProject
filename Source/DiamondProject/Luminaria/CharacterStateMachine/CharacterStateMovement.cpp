#include "CharacterStateMovement.h"
#include "CharacterStateJump.h"
#include "CharacterStateIdle.h"
#include "CharacterStateFall.h"
#include "CharacterStateAttract.h"
#include "CharacterStateDie.h"

#include <InputActionValue.h>

#include "../Actors/Absorber.h"
#include "../Core/DiamondProjectCharacter.h"
#include "../Core/DiamondProjectPlayerController.h"
#include "../SubSystems/PlayerManager.h"
#include "../SubSystems/InputUIManager.h"

#include "GameFramework/CharacterMovementComponent.h"

void UCharacterStateMovement::OnStateInit() {
	PlayerManager = GetCharacter()->GetWorld()->GetSubsystem<UPlayerManager>();
	InputUIManager = GetCharacter()->GetWorld()->GetSubsystem<UInputUIManager>();
}

void UCharacterStateMovement::OnStateBegin() {
	Controller = GetCharacter()->GetLuminariaController();
}

void UCharacterStateMovement::OnStateTick(float DeltaTime) {
	if (GetClass() == UCharacterStateMovement::StaticClass()) { // Pas besoin juste pas mettre de super dans le state jump 
		if (!GetCharacter()->GetCharacterMovement()->IsMovingOnGround()) {
			ChangeState(GetStateMachine()->StateFall);
		}
	}
}

void UCharacterStateMovement::OnMovement(const FInputActionValue& Value) {
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	if ((MovementVector.X >= 0.0F && MovementVector.X < 0.2F) || (MovementVector.X <= 0.0F && MovementVector.X > -0.2F)) {
		if (GetClass() == UCharacterStateMovement::StaticClass()) {
			ChangeState(GetStateMachine()->StateIdle);
		}
		return;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Red, FString::FromInt(GetLocalPlayer()->GetControllerId()));

	if (Controller->GetLocalPlayer()->GetControllerId() == 0) {
		//GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Yellow, TEXT("Keyboard & Gamepad"));
	}
	else if (Controller->GetLocalPlayer()->GetControllerId() == 1) {
		//GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Green, TEXT("Gamepad"));
	}

	if (MovementDirection.Y < 0) {
		if (bCanFlip) { // Valeur a linker avec la movable object 
			Controller->GetPawn()->SetActorRotation(FRotator(0, -90, 0), ETeleportType::None);

		}
	}
	else {
		if (bCanFlip) {
			Controller->GetPawn()->SetActorRotation(FRotator(0, 90, 0), ETeleportType::None);
		}
	}

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (ForwardDirection.X != 0) {
		MovementDirection = FVector(0.F, MovementVector.X, 0.f);
	}
	else if (ForwardDirection.Y != 0) {
		MovementDirection = FVector(MovementVector.X, 0.F, 0.F);
	}

	bool isCanceled = false;
	PlayerManager->OnPlayerMove.Broadcast(Cast<ADiamondProjectCharacter>(GetCharacter()), MovementVector, MovementDirection, isCanceled);

	if (!InputUIManager) {
		GEngine->AddOnScreenDebugMessage(-1, 5.F, FColor::Red, TEXT("No UI Subsystem"));
	}

	InputUIManager->OnPlayerMove();

	if (isCanceled) {
		return;
	}

	GetCharacter()->AddMovementInput(ForwardDirection, MovementVector.X);
}

void UCharacterStateMovement::OnJump() {
	ChangeState(GetStateMachine()->StateJump);
}

void UCharacterStateMovement::OnDie() {
	ChangeState(GetStateMachine()->StateDie);
}

void UCharacterStateMovement::OnAbsorberDetectCharacter(ADiamondProjectCharacter* Character, AAbsorber* Absorber) {
	ChangeState(GetStateMachine()->StateAttract);
}


