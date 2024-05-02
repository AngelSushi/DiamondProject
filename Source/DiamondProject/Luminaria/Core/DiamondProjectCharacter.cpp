// Copyright Epic Games, Inc. All Rights Reserved.

#include "DiamondProjectCharacter.h"

#include "DiamondProject/Luminaria/SubSystems/PlayerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "DiamondProject/Luminaria/Actors/Checkpoint.h"
#include "DiamondProject/Luminaria/Actors/CameraArea.h"
#include "Kismet/GameplayStatics.h"
#include "DiamondProject/Luminaria/Actors/LuminariaCamera.h"

#include "DiamondProject/Luminaria/CameraBehaviors/CameraBehavior.h"
#include "DiamondProject/Luminaria/CameraBehaviors/GoToBehavior.h"

#include "DiamondProject/Luminaria/CameraBehaviors/CameraDynamicBehavior.h"

ADiamondProjectCharacter::ADiamondProjectCharacter(){
	
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ADiamondProjectCharacter::BeginPlay() {
	Super::BeginPlay();
	
	PlayerManager = GetWorld()->GetSubsystem<UPlayerManager>();
	PlayerManager->RegisterPlayer(this);

	PlayerManager->OnPlayerUpdateCheckpoint.AddDynamic(this, &ADiamondProjectCharacter::OnPlayerUpdateCheckpoint);

	_checkPoint = GetActorLocation();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ADiamondProjectCharacter::OnBeginOverlap);

	MainCamera = Cast<ALuminariaCamera>(UGameplayStatics::GetActorOfClass(GetWorld(), ALuminariaCamera::StaticClass()));

	LastHitArea = MainCamera->CurrentArea;
}

void ADiamondProjectCharacter::Tick(float DeltaSeconds) {
    Super::Tick(DeltaSeconds);

	// Raycast pour détecter s'il est sur le ground ou pas 

	check(GetWorld());
	TArray<FHitResult> HitResults;

	FVector Start = GetActorLocation() + FVector::DownVector * GetSimpleCollisionRadius() / 2 * 3.5F;
	FVector End = Start + FVector::DownVector * 50.F;

	DrawDebugSphere(GetWorld(), Start, 10, 25, FColor::Orange);
	DrawDebugLine(GetWorld(), Start, End, FColor::Orange);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(MainCamera->CurrentArea);

	GetWorld()->LineTraceMultiByChannel(HitResults, Start, End, ECollisionChannel::ECC_Visibility,Params);

	bIsOnGround = HitResults.Num() > 0;

	if (bIsOnGround && !bIsOnGroundLastTick) {
		GroundActor = HitResults[0].GetActor();
		PlayerManager->OnPlayerLandOnGround.Broadcast(this);
	}

	bIsOnGroundLastTick = bIsOnGround;
}

void ADiamondProjectCharacter::Death(EDeathCause DeathCause) {
	FTimerHandle RespawnTimer;

	if (_checkPoint != FVector::Zero()) {
		SetActorLocation(_checkPoint);
	}
	
	PlayerManager->OnPlayerDeath.Broadcast(this,DeathCause);
}

void ADiamondProjectCharacter::UpdateCheckpoint(ACheckpoint* checkpoint) {
	_checkPoint = checkpoint->checkPoint->GetComponentLocation();
	PlayerManager->OnPlayerUpdateCheckpoint.Broadcast(this, checkpoint);
}

void ADiamondProjectCharacter::OnPlayerUpdateCheckpoint(ADiamondProjectCharacter* Character, ACheckpoint* Checkpoint) {
	if (Character != this) {
		_checkPoint = Checkpoint->checkPoint->GetComponentLocation();
	}
}

void ADiamondProjectCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (ACameraArea* HitArea = Cast<ACameraArea>(OtherActor)) {
		if (HitArea->AreaBehavior != LastHitArea->AreaBehavior) {
			MainCamera->CurrentArea = HitArea;

			ECameraBehavior TargetBehavior = HitArea->AreaBehavior;

			if (TargetBehavior == ECameraBehavior::DEFAULT) {
				TargetBehavior = ECameraBehavior::GOTO;
			}

			ADiamondProjectCharacter* OtherPlayer = PlayerManager->GetOtherPlayer(this);

			if (HitArea->PlayerNeeded == 2) {	
				if (OtherPlayer->LastHitArea == HitArea) {
					// Faire le switch
					MainCamera->SwitchBehavior(TargetBehavior, [&HitArea,&OtherPlayer,this](UCameraBehavior* Behavior) {
						if (UGoToBehavior* GoTo = Cast<UGoToBehavior>(Behavior)) {
							GoTo->NextBehavior = ECameraBehavior::DEFAULT;
							GoTo->GoTo = HitArea->GoTo->GetComponentLocation();
							GoTo->Speed = 500.F;
						}

						if (UCameraDynamicBehavior* DynamicBehavior = Cast<UCameraDynamicBehavior>(Behavior)) {
							FVector Barycenter = (GetActorLocation() - OtherPlayer->GetActorLocation()) / 2;
							Barycenter.X = HitArea->ZoomMin;

							DynamicBehavior->SetBarycenter(Barycenter);
							//
						}
					});
				}
			}
			else {
				MainCamera->SwitchBehavior(TargetBehavior, [&HitArea,&OtherPlayer,this](UCameraBehavior* Behavior) {
					if (UGoToBehavior* GoTo = Cast<UGoToBehavior>(Behavior)) {
						GoTo->NextBehavior = ECameraBehavior::DEFAULT;
						GoTo->GoTo = HitArea->GoTo->GetComponentLocation();
						GoTo->Speed = 500.F;
					}

					if (UCameraDynamicBehavior* DynamicBehavior = Cast<UCameraDynamicBehavior>(Behavior)) {
						FVector Barycenter = (GetActorLocation() - OtherPlayer->GetActorLocation()) / 2;
						Barycenter.X = HitArea->ZoomMin;

						DynamicBehavior->SetBarycenter(Barycenter);
						//
					}
				});
			}

			LastHitArea = HitArea;
		}
	}
}
