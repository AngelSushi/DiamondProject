#include "LuminariaCamera.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerManager.h"
#include "DiamondProject/Luminaria/CameraBehaviors/GoToBehavior.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"

#include "DiamondProject/Luminaria/CameraBehaviors/CameraDefaultBehavior.h"
#include "DiamondProject/Luminaria/CameraBehaviors/CameraLeaderBehavior.h"
#include "DiamondProject/Luminaria/CameraBehaviors/CameraDynamicBehavior.h"
#include "DiamondProject/Luminaria/CameraBehaviors/GoToBehavior.h"
#include "DiamondProject/Luminaria/CameraBehaviors/HeightCameraBehavior.h"

#include "DiamondProject/Luminaria/Actors/CameraArea.h"

ALuminariaCamera::ALuminariaCamera() {
	PrimaryActorTick.bCanEverTick = true;
}

void ALuminariaCamera::BeginPlay() {
	Super::BeginPlay();

	UPlayerManager* PlayerManager = GetWorld()->GetSubsystem<UPlayerManager>();
	PlayerManager->OnPlayerRegister.AddDynamic(this, &ALuminariaCamera::OnPlayerRegister);
	PlayerManager->OnPlayerDeath.AddDynamic(this, &ALuminariaCamera::OnPlayerDeath);

	StartPosition = GetActorLocation();

	InitBehavior();

	if (CurrentArea) {
		CurrentArea->SetVisited(true);
	}

}

void ALuminariaCamera::InitBehavior() {
	SwitchBehavior(BehaviorState);
}

// Utiliser Lerp et InverseLerp pour faire le zoomMax et la distanceMax du lien

void ALuminariaCamera::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (CameraBehavior) {
		CameraBehavior->TickBehavior(DeltaTime);
	
		if (HeightBehavior) {
			HeightBehavior->TickBehavior(DeltaTime);
		}
	}
}

void ALuminariaCamera::OnPlayerRegister(ADiamondProjectCharacter* Character) {
	Characters.Add(Character);
}

void ALuminariaCamera::SwitchBehavior(ECameraBehavior SwitchBehavior, TFunction<void(UCameraBehavior* AddedComponent)> ResultFunc /*= [](UCameraBehavior* CameraBehavior) {}*/) {

	if (CameraBehavior && BehaviorState == SwitchBehavior) {
		UE_LOG(LogTemp, Error, TEXT("The camera has already this behavior."));
		return;
	}

	//CameraBehavior = NewObject<UCameraBehavior>(Behavior); Doesn't work with child functions

	switch (SwitchBehavior) {
		case ECameraBehavior::DEFAULT:
			DefaultBehavior = NewObject<UCameraDefaultBehavior>();
			CameraBehavior = DefaultBehavior;
			break;

		case ECameraBehavior::DYNAMIC:
			DynamicBehavior = NewObject<UCameraDynamicBehavior>();
			HeightBehavior = NewObject<UHeightCameraBehavior>();
			CameraBehavior = DynamicBehavior;
			break;

		case ECameraBehavior::GOTO:
			GoToBehavior = NewObject<UGoToBehavior>();
			CameraBehavior = GoToBehavior;
			break;

		case ECameraBehavior::LEADER:
			LeaderBehavior = NewObject<UCameraLeaderBehavior>();
			CameraBehavior = LeaderBehavior;
			break;
	}

	CameraBehavior->BeginBehavior(this);

	if (HeightBehavior) {
		HeightBehavior->BeginBehavior(this);
	}

	ResultFunc(CameraBehavior);
}

void ALuminariaCamera::OnPlayerDeath(ADiamondProjectCharacter* Character,EDeathCause DeathCause) {
	ECameraBehavior CurrentBehavior = BehaviorState;

	if (CurrentBehavior == ECameraBehavior::DEFAULT) {
		return;
	}

	if (bHasDead) {
		return;
	}
	
	bHasDead = true;

	BehaviorState = ECameraBehavior::GOTO;

	FTimerHandle Timer;
	
	SwitchBehavior(BehaviorState, [this,CurrentBehavior](UCameraBehavior* Component) {
		if (UGoToBehavior* GoToBehaviorComponent = Cast<UGoToBehavior>(Component)) {
			FVector GoTo = FVector::Zero();

			for (ADiamondProjectCharacter* ACharacter : Characters) {
				GoTo += ACharacter->GetActorLocation();
			}

			GoTo /= Characters.Num();
			GoTo.X = StartPosition.X;
			GoTo.Z = StartPosition.Z;

			GoToBehaviorComponent->GoTo = GoTo;
			GoToBehaviorComponent->Speed = 1000.F;
			GoToBehaviorComponent->NextBehavior = CurrentBehavior;
		}
	});
}
