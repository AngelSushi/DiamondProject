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

#include "DiamondProject/Luminaria/Core/DiamondProjectPlayerController.h"

#include "Components/PointLightComponent.h"

ADiamondProjectCharacter::ADiamondProjectCharacter(){
	
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	//GetCharacterMovement()->bNotifyApex = true;
	
	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Energy"));
	Light->SetupAttachment(RootComponent);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ADiamondProjectCharacter::BeginPlay() {
	Super::BeginPlay();
	
	PlayerManager = GetWorld()->GetSubsystem<UPlayerManager>();

	FTimerHandle RegisterTimer;

	GetWorld()->GetTimerManager().SetTimer(RegisterTimer,[this]() {
		PlayerManager->RegisterPlayer(this);
		GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Orange, TEXT("Begin Player"));
	},0.1f,false);

	PlayerManager->OnPlayerUpdateCheckpoint.AddDynamic(this, &ADiamondProjectCharacter::OnPlayerUpdateCheckpoint);

	_checkPoint = GetActorLocation();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ADiamondProjectCharacter::OnBeginOverlap);

	MainCamera = Cast<ALuminariaCamera>(UGameplayStatics::GetActorOfClass(GetWorld(), ALuminariaCamera::StaticClass()));

	LastHitArea = MainCamera->CurrentArea;
	bIsOnGround = true;
}

void ADiamondProjectCharacter::Tick(float DeltaSeconds) {
    Super::Tick(DeltaSeconds);
}

void ADiamondProjectCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) {
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if (PrevMovementMode == EMovementMode::MOVE_Walking) {
		bIsOnGround = false;

		if (GetLuminariaController()->IsJumping()) {
			GetCharacterMovement()->bNotifyApex = true;
		}
	}
}

void ADiamondProjectCharacter::Landed(const FHitResult& Hit) {
	bIsOnGround = true;
	PlayerManager->OnPlayerLandOnGround.Broadcast(this);

	if (GetLuminariaController()->IsJumping()) {
		GetLuminariaController()->SetJumping(false);
	}
}

void ADiamondProjectCharacter::Death(EDeathCause DeathCause) { // CHeck ce que fait la mort ya ptetre de le faire en respawn
	PlayerManager->OnPlayerDeath.Broadcast(this,DeathCause);
}

void ADiamondProjectCharacter::Respawn(EDeathCause DeathCause) {
	if (_checkPoint != FVector::Zero()) {
		SetActorLocation(_checkPoint);
	}

	PlayerManager->OnPlayerRespawn.Broadcast(this, DeathCause, _checkPoint);
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
		HitArea->SetVisited(true);
		

		ECameraBehavior TargetBehavior = HitArea->AreaBehavior;

		if (TargetBehavior == ECameraBehavior::DEFAULT) {
			TargetBehavior = ECameraBehavior::GOTO;
		}

		ADiamondProjectCharacter* OtherPlayer = PlayerManager->GetOtherPlayer(this);

		//GEngine->AddOnScreenDebugMessage(-1, 5.F, FColor::Orange, FString::Printf(TEXT("Hit Player Name %s"), *GetActorNameOrLabel()));
		//GEngine->AddOnScreenDebugMessage(-1, 5.F, FColor::Orange, FString::Printf(TEXT("Other Player Name %s"), *OtherPlayer->GetActorNameOrLabel()));

		if (HitArea->PlayerNeeded == 2) {	

			//GEngine->AddOnScreenDebugMessage(-1, 5.F, FColor::Orange, TEXT("001"));
			if (OtherPlayer->LastHitArea == HitArea) {

				//GEngine->AddOnScreenDebugMessage(-1, 5.F, FColor::Orange, TEXT("002"));
				// Faire le switch
				MainCamera->CurrentArea = HitArea;

				if (TargetBehavior != LastHitArea->AreaBehavior) {

				//	GEngine->AddOnScreenDebugMessage(-1, 5.F, FColor::Orange, FString::Printf(TEXT("Enum %s Name %s"), *UEnum::GetDisplayValueAsText(TargetBehavior).ToString(),*HitArea->GetActorNameOrLabel()));

					MainCamera->SwitchBehavior(TargetBehavior, [&HitArea, &OtherPlayer, this](UCameraBehavior* Behavior) {
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
		}
		else {
			MainCamera->CurrentArea = HitArea;

			if (TargetBehavior != LastHitArea->AreaBehavior) {
				MainCamera->SwitchBehavior(TargetBehavior, [&HitArea, &OtherPlayer, this](UCameraBehavior* Behavior) {
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

		LastHitArea = HitArea;
	}
	
}

void ADiamondProjectCharacter::OnLandOnGround(ADiamondProjectCharacter* Character) {
	if (ADiamondProjectPlayerController* PlayerController = Cast<ADiamondProjectPlayerController>(GetController())) {
		PlayerController->bIsJumping = false;
	}
}


