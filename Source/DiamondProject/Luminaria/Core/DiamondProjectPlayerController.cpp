// Copyright Epic Games, Inc. All Rights Reserved.

#include "DiamondProjectPlayerController.h"
#include "GameFramework/Pawn.h"
#include "DiamondProjectCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerEventsDispatcher.h"
#include "Engine/LocalPlayer.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ADiamondProjectPlayerController::ADiamondProjectPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}


void ADiamondProjectPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(PlayerMappingContext, 0);
	}

	PlayerEventsDispatcher = GetWorld()->GetSubsystem<UPlayerEventsDispatcher>();
}


void ADiamondProjectPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction,ETriggerEvent::Triggered,this,&ADiamondProjectPlayerController::Move);

		EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Started,this,&ADiamondProjectPlayerController::Jump);
		EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Completed,this,&ADiamondProjectPlayerController::StopJump);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ADiamondProjectPlayerController::Move(const FInputActionValue& Value) {
	FVector2D MovementVector = Value.Get<FVector2D>();


	if (MovementDirection.Y < 0)
	{
		if (bCanFlip == true) 
		{
			GetPawn()->SetActorRotation(FRotator(0, -90, 0), ETeleportType::None);

		}

		/*if (bIsLookingLeft == false)
		{
			bIsLookingLeft = true;
		}*/
	}
	else
	{
		if (bCanFlip == true)
		{
			GetPawn()->SetActorRotation(FRotator(0, 90, 0), ETeleportType::None);

		}
		/*if (bIsLookingLeft == true)
		{
			bIsLookingLeft = false;
		}*/

	}


	
	// find out which way is forward
	const FRotator Rotation =GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (ForwardDirection.X != 0) {
		MovementDirection = FVector(0.F, MovementVector.X, 0.f);
	}
	else if (ForwardDirection.Y != 0) {
		MovementDirection = FVector(MovementVector.X,0.F, 0.F);
	}


	
	bool isCanceled = false;
	PlayerEventsDispatcher->OnPlayerMove.Broadcast(Cast<ADiamondProjectCharacter>(GetCharacter()), MovementDirection, isCanceled);

	if (isCanceled) {
		return;
	}

	if(isUsingDepthMovement)
	{
		GetCharacter()->AddMovementInput(ForwardDirection, MovementVector.Y);
		GetCharacter()->AddMovementInput(RightDirection, MovementVector.X);	
	}
	else
	{
		GetCharacter()->AddMovementInput(ForwardDirection, MovementVector.X);
	}

}

void ADiamondProjectPlayerController::Jump()
{
	GetCharacter()->Jump();
}

void ADiamondProjectPlayerController::StopJump()
{
	GetCharacter()->StopJumping();
}
