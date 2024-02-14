// Copyright Epic Games, Inc. All Rights Reserved.

#include "DiamondProjectCharacter.h"

#include "DiamondProject/Luminaria/SubSystems/PlayerEventsDispatcher.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "DiamondProject/Luminaria/Actors/Checkpoint.h"

ADiamondProjectCharacter::ADiamondProjectCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ADiamondProjectCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerEventsDispatcher = GetWorld()->GetSubsystem<UPlayerEventsDispatcher>();
	PlayerEventsDispatcher->OnPlayerRegister.Broadcast(this);
}

void ADiamondProjectCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void ADiamondProjectCharacter::Death()
{
	GetMesh()->SetVisibility(false);

	FTimerHandle RespawnTimer;

	if (_checkPoint != FVector::Zero()) {
		GetWorld()->GetTimerManager().SetTimer(RespawnTimer, [this, &RespawnTimer]() {
			SetActorLocation(_checkPoint);
			GetMesh()->SetVisibility(true);
			GetWorld()->GetTimerManager().ClearTimer(RespawnTimer);
		}, 3.F, false);
	}

	PlayerEventsDispatcher->OnPlayerDeath.Broadcast(this);
}

void ADiamondProjectCharacter::UpdateCheckpoint(ACheckpoint* checkpoint) {
	_checkPoint = checkpoint->checkPoint->GetComponentLocation();
	PlayerEventsDispatcher->OnPlayerUpdateCheckpoint.Broadcast(this, checkpoint);
}