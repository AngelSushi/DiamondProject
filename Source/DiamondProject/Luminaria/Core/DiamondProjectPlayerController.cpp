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
#include "../CharacterStateMachine/CharacterStateMachine.h"

#include "../CharacterStateMachine/CharacterStateJump.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ADiamondProjectPlayerController::ADiamondProjectPlayerController() {
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}


void ADiamondProjectPlayerController::BeginPlay() {
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

	if (GetLocalPlayer()) {
		//GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Green, TEXT("Has Player Local"));
	}
	else {
		//GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Red, FString::Printf(TEXT("Has No Player Local For %s"),*GetActorNameOrLabel()));
	}

	//JumpMaxDuration = GetPlayer()->GetPlayerAsset()->JumpMaxDuration;
	//JumpMinDuration = GetPlayer()->GetPlayerAsset()->JumpMinDuration;

	if (GetPlayer()->GetStateMachine() && GetPlayer()->GetStateMachine()->StateJump) { // A MODIFIER DIRECT DEPUISLE STATE 
		GetPlayer()->GetStateMachine()->StateJump->SetJumpMinDuration(GetPlayer()->GetPlayerAsset()->JumpMinDuration);
		GetPlayer()->GetStateMachine()->StateJump->SetJumpMaxDuration(GetPlayer()->GetPlayerAsset()->JumpMaxDuration);
	}


	// Set Le deuxieme local player id sur 1
	// 
	//GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Red, FString::Printf(TEXT("From %s"), *GetPawn()->GetActorNameOrLabel()));
}


void ADiamondProjectPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		//if (GetLocalPlayer() && GetLocalPlayer()->GetControllerId() == 0) { // Keyboard
			EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ADiamondProjectPlayerController::Move);
			EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Completed, this, &ADiamondProjectPlayerController::Move);
			
			
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ADiamondProjectPlayerController::Jump);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ADiamondProjectPlayerController::OnInputJumpReleased);
			
			//GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Red, TEXT("Keyboard"));
		//}
		//else if (GetLocalPlayer()->GetControllerId() == 1) { // Gamepad
			//EnhancedInputComponent->BindAction(MovementActionGamepad, ETriggerEvent::Triggered, this, &ADiamondProjectPlayerController::Move);
			//EnhancedInputComponent->BindAction(MovementActionGamepad, ETriggerEvent::Completed, this, &ADiamondProjectPlayerController::Move);

			//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ADiamondProjectPlayerController::Jump);
			//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ADiamondProjectPlayerController::OnInputJumpReleased);

			
			//GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Blue, TEXT("Gamepad"));
		//}
	}
	else {
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ADiamondProjectPlayerController::Move(const FInputActionValue& Value) {
	GetPlayer()->GetStateMachine()->OnMovement(Value);
}

void ADiamondProjectPlayerController::Jump() {
	GetPlayer()->GetStateMachine()->OnJump();
}

void ADiamondProjectPlayerController::OnInputJumpReleased() {
	GetPlayer()->GetStateMachine()->OnInputJumpReleased();
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