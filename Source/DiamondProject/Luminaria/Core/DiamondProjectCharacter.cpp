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
#include "DiamondProject/Luminaria/DataAssets/PlayerAsset.h"

#include "DiamondProject/Luminaria/CharacterStateMachine/CharacterStateMachine.h"


#include "../UMG/UIComboInput.h"

#include "../SubSystems/InputUIManager.h"
#include "../SubSystems/UISubsystem.h"

#include "NiagaraComponent.h"

ADiamondProjectCharacter::ADiamondProjectCharacter() {

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

	DeathRespawnParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DeathRespawnParticle"));
	DeathRespawnParticle->SetupAttachment(GetMesh());

	LandOnGroundParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LandOnGroundParticle"));
	LandOnGroundParticle->SetupAttachment(GetMesh());
}

void ADiamondProjectCharacter::BeginPlay() {
	Super::BeginPlay();
	
	PlayerManager = GetWorld()->GetSubsystem<UPlayerManager>();
	UISystem = GetWorld()->GetSubsystem<UUISubsystem>();
	InputUIManager = GetWorld()->GetSubsystem<UInputUIManager>();

	EnableInputListener();
	InputUIManager->OnInputComplete.AddDynamic(this, &ADiamondProjectCharacter::CompleteInput);

	FTimerHandle RegisterTimer;

	GetWorld()->GetTimerManager().SetTimer(RegisterTimer,[this]() {
		PlayerManager->RegisterPlayer(this);
	},0.1f,false); // 0.1f

	PlayerManager->OnPlayerUpdateCheckpoint.AddDynamic(this, &ADiamondProjectCharacter::OnPlayerUpdateCheckpoint);

	_checkPoint = GetActorLocation();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ADiamondProjectCharacter::OnBeginOverlap);

	MainCamera = Cast<ALuminariaCamera>(UGameplayStatics::GetActorOfClass(GetWorld(), ALuminariaCamera::StaticClass()));

	LastHitArea = MainCamera->CurrentArea;
	bIsOnGround = true;


	GetCharacterMovement()->MaxWalkSpeed = GetPlayerAsset()->Speed;

	CharacterStateMachine = NewObject<UCharacterStateMachine>(/*TEXT("StateMachine")*/);
	CharacterStateMachine->SMInit(this);
	CharacterStateMachine->SMBegin();

}

void ADiamondProjectCharacter::Tick(float DeltaSeconds) {
    Super::Tick(DeltaSeconds);

	MinEnergy = GetPlayerAsset()->MinEnergy;
	MaxEnergy = GetPlayerAsset()->MaxEnergy;

	SpeedIncrease = GetPlayerAsset()->SpeedIncrease;
	JumpDurationIncrease = GetPlayerAsset()->JumpDurationIncrease;

	CharacterStateMachine->SMTick(DeltaSeconds);
}

void ADiamondProjectCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) {
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if (PrevMovementMode == EMovementMode::MOVE_Walking) {
		bIsOnGround = false;

		if (GetLuminariaController() && GetLuminariaController()->IsJumping()) {
			GetCharacterMovement()->bNotifyApex = true;
		}
	}
}

void ADiamondProjectCharacter::Landed(const FHitResult& Hit) {
	bIsOnGround = true;
	PlayerManager->OnPlayerLandOnGround.Broadcast(this);
	GroundActor = Hit.GetActor();
	
	FTimerHandle AnimationTimer;

	GetWorld()->GetTimerManager().SetTimer(AnimationTimer, [this]() {
		FVector Position = GetActorLocation();
		Position.Z -= GetSimpleCollisionHalfHeight();

		LandOnGroundParticle->SetWorldLocation(Position);
		LandOnGroundParticle->ActivateSystem();

	}, 0.1f, false);
}

void ADiamondProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindKey(EKeys::A, IE_Pressed, this, &ADiamondProjectCharacter::AbsorberInputStarted);
	PlayerInputComponent->BindKey(EKeys::E, IE_Pressed, this, &ADiamondProjectCharacter::AbsorberInputStarted);
	
	
	PlayerInputComponent->BindKey(EKeys::Gamepad_FaceButton_Bottom, IE_Pressed, this, &ADiamondProjectCharacter::AbsorberInputStarted);
	PlayerInputComponent->BindKey(EKeys::Gamepad_FaceButton_Top, IE_Pressed, this, &ADiamondProjectCharacter::AbsorberInputStarted);
	PlayerInputComponent->BindKey(EKeys::Gamepad_FaceButton_Right, IE_Pressed, this, &ADiamondProjectCharacter::AbsorberInputStarted);
	PlayerInputComponent->BindKey(EKeys::Gamepad_FaceButton_Left, IE_Pressed, this, &ADiamondProjectCharacter::AbsorberInputStarted);

}

void ADiamondProjectCharacter::AbsorberInputStarted(FKey Key) {
	GetStateMachine()->OnAbsorberInputStarted(Key);
}

void ADiamondProjectCharacter::Death(EDeathCause DeathCause) { // CHeck ce que fait la mort ya ptetre de le faire en respawn
	PlayerManager->OnPlayerDeath.Broadcast(this,DeathCause);

	FTimerHandle RespawnHandle;
	GetStateMachine()->OnDie();

	
	
	GEngine->AddOnScreenDebugMessage(-1, 10.F, FColor::Orange, UEnum::GetValueAsString(DeathCause));
	//Respawn(DeathCause);

	GetWorld()->GetTimerManager().SetTimer(RespawnHandle, [&]() {
		Respawn(DeathCause);
	},3.F, false);
}

void ADiamondProjectCharacter::Respawn(EDeathCause DeathCause) {
	if (_checkPoint != FVector::Zero()) {
		GEngine->AddOnScreenDebugMessage(-1, 100.F, FColor::Cyan, FString::Printf(TEXT("Respawn Position %s For %s"), *_checkPoint.ToString(),*GetActorNameOrLabel()));
		SetActorLocation(_checkPoint);
	}

	PlayerManager->OnPlayerRespawn.Broadcast(this, DeathCause, _checkPoint);
}

void ADiamondProjectCharacter::UpdateCheckpoint(ACheckpoint* checkpoint) {
	PlayerManager->OnPlayerUpdateCheckpoint.Broadcast(this, checkpoint);
}

void ADiamondProjectCharacter::OnPlayerUpdateCheckpoint(ADiamondProjectCharacter* Character, ACheckpoint* Checkpoint) {
	if (Character != this) {
		_checkPoint = Checkpoint->checkPoint->GetComponentLocation();
	}
}

void ADiamondProjectCharacter::EnableInputListener() {
	InputUIManager->EnableInput(ADiamondProjectCharacter::StaticClass());

	ComboWidget = CreateWidget<UUIComboInput>(GetWorld(), UISystem->GetUIAsset()->ComboInputClass);
	ComboWidget->InitComboUI({ EInput::Y }, FText::FromString(TEXT("Sauter")));

	int32 ScreenX;
	int32 ScreenY;

	GetWorld()->GetFirstPlayerController()->GetViewportSize(ScreenX, ScreenY);
	ScreenY -= 600.F;
	ScreenX -= 200.F;

	ComboWidget->SetPositionInViewport(FVector2D(ScreenX / 2, ScreenY / 2));
	ComboWidget->AddToViewport();
}

void ADiamondProjectCharacter::DisableInputListener() {
	if (!ComboWidget) {
		return;
	}

	InputUIManager->DisableInput(GetClass());
	ComboWidget->RemoveFromViewport();
}

void ADiamondProjectCharacter::CompleteInput(UInputUI* Input) { // Add Event
	if (Input->GetClass()->GetName().Equals(ADiamondProjectCharacter::StaticClass()->GetName())) {
		ComboWidget->RemoveFromViewport();
	}
}

void ADiamondProjectCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (ACameraArea* HitArea = Cast<ACameraArea>(OtherActor)) {
		HitArea->SetVisited(true);
		
		ECameraBehavior OriginBehavior = HitArea->AreaBehavior;
		ECameraBehavior TargetBehavior = HitArea->AreaBehavior;

		if (TargetBehavior == ECameraBehavior::DEFAULT /* || TargetBehavior == ECameraBehavior::DYNAMIC */) {
			TargetBehavior = ECameraBehavior::GOTO;
		}

		ADiamondProjectCharacter* OtherPlayer = PlayerManager->GetOtherPlayer(this);

		if (!OtherPlayer) {
			return;
		}

		if (HitArea->PlayerNeeded == 2) {	
			if (OtherPlayer->LastHitArea == HitArea) {
				GEngine->AddOnScreenDebugMessage(-1, 5.F, FColor::Yellow, TEXT("Switch"));
				// Faire le switch
				MainCamera->CurrentArea = HitArea;

				if (TargetBehavior != LastHitArea->AreaBehavior) {
					MainCamera->SwitchBehavior(TargetBehavior, [&HitArea, &OtherPlayer, this,&OriginBehavior](UCameraBehavior* Behavior) {
						

						GEngine->AddOnScreenDebugMessage(-1, 5.F, FColor::Red, TEXT("OnSwitchBehavior"));
						if (UGoToBehavior* GoTo = Cast<UGoToBehavior>(Behavior)) {
							
							if (OriginBehavior == ECameraBehavior::DEFAULT) {
								GEngine->AddOnScreenDebugMessage(-1, 5.F, FColor::Red, TEXT("Default Behavior"));
								GoTo->NextBehavior = ECameraBehavior::DEFAULT;
								if (HitArea->GoTo) {
									GoTo->GoTo = HitArea->GoTo->GetComponentLocation();
								}
							}
							/*else if (OriginBehavior == ECameraBehavior::DYNAMIC) {
								FVector Barycenter = (GetActorLocation() + OtherPlayer->GetActorLocation()) / 2;

								if (HitArea->ZoomMin > HitArea->ZoomMax) { // For Some Reason, In Certain Level ZoomMin is Greater Than ZoomMax. We Manage This Case
									Barycenter.X = FMath::Clamp(Barycenter.X, HitArea->ZoomMax, HitArea->ZoomMin);
								}
								else {
									Barycenter.X = FMath::Clamp(Barycenter.X, HitArea->ZoomMin, HitArea->ZoomMax);
								}

								Barycenter.Z = GoTo->DefaultZ;

								GoTo->GoTo = Barycenter;
								GoTo->NextBehavior = ECameraBehavior::DYNAMIC;
							}
							*/
						}
					});
				}
			}
		}
		else {
			MainCamera->CurrentArea = HitArea;
			
			if (TargetBehavior != LastHitArea->AreaBehavior) {
				MainCamera->SwitchBehavior(TargetBehavior, [&HitArea,&OtherPlayer, this,&OriginBehavior](UCameraBehavior* Behavior) {
					
					if (UGoToBehavior* GoTo = Cast<UGoToBehavior>(Behavior)) {
						if (OriginBehavior == ECameraBehavior::DEFAULT) {
							GoTo->NextBehavior = ECameraBehavior::DEFAULT;
							GoTo->GoTo = HitArea->GoTo->GetComponentLocation();
						}
						/*else if (OriginBehavior == ECameraBehavior::DYNAMIC) {
							GEngine->AddOnScreenDebugMessage(-1, 5.F, FColor::Blue, TEXT("Dynamic Behavior"));
							FVector Barycenter = (GetActorLocation() + OtherPlayer->GetActorLocation()) / 2;

							if (HitArea->ZoomMin > HitArea->ZoomMax) { // For Some Reason, In Certain Level ZoomMin is Greater Than ZoomMax. We Manage This Case
								Barycenter.X = FMath::Clamp(Barycenter.X, HitArea->ZoomMax, HitArea->ZoomMin);
							}
							else {
								Barycenter.X = FMath::Clamp(Barycenter.X, HitArea->ZoomMin, HitArea->ZoomMax);
							}

							Barycenter.Z = GoTo->DefaultZ;

							GoTo->GoTo = Barycenter;
							GoTo->NextBehavior = ECameraBehavior::DYNAMIC;
						}*/
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


