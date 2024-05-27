// Copyright Epic Games, Inc. All Rights Reserved.

#include "DiamondProjectPlayerController.h"
#include "GameFramework/Pawn.h"
#include "DiamondProjectCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerManager.h"
#include "DiamondProject/Luminaria/SubSystems/MapManager.h"
#include "Engine/LocalPlayer.h"
#include "Blueprint/UserWidget.h"
#include "DiamondProject/Luminaria/UMG/MapWidget.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "DiamondProject/Luminaria/DataAssets/PlayerAsset.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ADiamondProjectPlayerController::ADiamondProjectPlayerController() {
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

	PlayerManager = GetWorld()->GetSubsystem<UPlayerManager>();
}

void ADiamondProjectPlayerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (!GetPlayer()) {
		return;
	}

	JumpMaxDuration = GetPlayer()->GetPlayerAsset()->JumpMaxDuration;
	JumpMinDuration = GetPlayer()->GetPlayerAsset()->JumpMinDuration;

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


void ADiamondProjectPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		if (GetLocalPlayer()->GetControllerId() == 0) { // Keyboard
			EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ADiamondProjectPlayerController::Move);
			EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Completed, this, &ADiamondProjectPlayerController::Move);
			
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ADiamondProjectPlayerController::Jump);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ADiamondProjectPlayerController::OnInputJumpReleased);

			GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Red, FString::Printf(TEXT("From Keyboard %s"),*GetActorNameOrLabel()));
		}
		else if (GetLocalPlayer()->GetControllerId() == 1) { // Gamepad
			EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ADiamondProjectPlayerController::Move);
			EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Completed, this, &ADiamondProjectPlayerController::Move);

			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ADiamondProjectPlayerController::Jump);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ADiamondProjectPlayerController::OnInputJumpReleased);


			GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Blue, FString::Printf(TEXT("Gamepad from %s"),*GetActorNameOrLabel()));
		}
		
	}
	else {
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ADiamondProjectPlayerController::Move(const FInputActionValue& Value) {
	FVector2D MovementVector = Value.Get<FVector2D>();
	if ((MovementVector.X > 0.0F && MovementVector.X < 0.2F)  || (MovementVector.X < 0.0F && MovementVector.X > -0.2F)) {
		return;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Yellow, TEXT("Ask For Movement"));


	MoveValue = MovementVector;
	// Check si la distance est bonne

	if (MovementDirection.Y < 0) {
		if (bCanFlip == true) {
			GetPawn()->SetActorRotation(FRotator(0, -90, 0), ETeleportType::None);

		}
	}
	else {
		if (bCanFlip == true) {
			GetPawn()->SetActorRotation(FRotator(0, 90, 0), ETeleportType::None);

		}
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
	PlayerManager->OnPlayerMove.Broadcast(Cast<ADiamondProjectCharacter>(GetCharacter()),MovementVector, MovementDirection, isCanceled);

	if (isCanceled) {
		return;
	}

	GetCharacter()->AddMovementInput(ForwardDirection, MovementVector.X);
}

void ADiamondProjectPlayerController::Jump() {
	GetCharacter()->Jump();
	bIsJumping = true;
	bIsJumpPressed = true;

	GetCharacter()->GetCharacterMovement()->GravityScale = 0.F;
	JumpTimer = 0.F;
}

void ADiamondProjectPlayerController::StopJump() {
	GetCharacter()->GetCharacterMovement()->GravityScale = GetPlayer()->GetGravityScaleSaved();
}

void ADiamondProjectPlayerController::OnInputJumpReleased() {
	if (JumpTimer > JumpMinDuration) {
		StopJump();
	}

	bIsJumpPressed = false;
}

void ADiamondProjectPlayerController::OpenMap() {
	UMapManager* MapManager = GetWorld()->GetSubsystem<UMapManager>();
	ADiamondProjectCharacter* LeaderMapCharacter = Cast<ADiamondProjectCharacter>(GetCharacter());

	if (!bIsMapOpen) {
		if (LeaderMapCharacter->MapWidgetClass) {
			LeaderMapCharacter->MapWidget = CreateWidget<UMapWidget>(GetWorld(), LeaderMapCharacter->MapWidgetClass);
			LeaderMapCharacter->MapWidget->AddToViewport();
			MapManager->OpenMap(LeaderMapCharacter);
		}
	}
	else {
		if (LeaderMapCharacter->MapWidget) {
			MapManager->CloseMap(LeaderMapCharacter);
		}
	}

	bIsMapOpen = !bIsMapOpen;
}

/*void ADiamondProjectPlayerController::Push() {
	bIsPushing = true;
}

void ADiamondProjectPlayerController::StopPush() {
	GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Black, TEXT("Stop Push"));
	bIsPushing = false;
}

void ADiamondProjectPlayerController::Pull() {
	bIsPulling = true;
}

void ADiamondProjectPlayerController::StopPull() {
	bIsPulling = false;
}
*/