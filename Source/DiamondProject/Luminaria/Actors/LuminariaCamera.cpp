#include "LuminariaCamera.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerManager.h"
#include "DiamondProject/Luminaria/CameraBehaviors/GoToBehavior.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"

#include "DiamondProject/Luminaria/CameraBehaviors/CameraDefaultBehavior.h"
#include "DiamondProject/Luminaria/CameraBehaviors/CameraLeaderBehavior.h"
#include "DiamondProject/Luminaria/CameraBehaviors/CameraDynamicBehavior.h"
#include "DiamondProject/Luminaria/CameraBehaviors/GoToBehavior.h"
#include "DiamondProject/Luminaria/CameraBehaviors/HeightCameraBehavior.h"
#include "../CameraBehaviors/CameraFollowBehavior.h"

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

	if (CurrentArea) {
		CurrentArea->TickArea(DeltaTime);
	}

	if (HeightBehavior) {
		//HeightBehavior->TickBehavior(DeltaTime);
	}

	if (CameraBehavior) {
		CameraBehavior->TickBehavior(DeltaTime);
	}
}

void ALuminariaCamera::OnPlayerRegister(ADiamondProjectCharacter* Character) {
	Characters.Add(Character);
}

void ALuminariaCamera::SwitchBehavior(ECameraBehavior SwitchBehavior, TFunction<void(UCameraBehavior* AddedComponent)> ResultFunc /*= [](UCameraBehavior* CameraBehavior) {}*/) {
	SwitchBehaviorFromBlueprint(SwitchBehavior);
	ResultFunc(CameraBehavior);
}

void ALuminariaCamera::SwitchBehaviorFromBlueprint(ECameraBehavior SwitchBehavior) {
	if (CameraBehavior && BehaviorState == SwitchBehavior) {
		UE_LOG(LogTemp, Error, TEXT("The camera has already this behavior."));
		return;
	}

	//CameraBehavior = NewObject<UCameraBehavior>(Behavior); Doesn't work with child functions
	BehaviorState = SwitchBehavior;

	switch (SwitchBehavior) {
	case ECameraBehavior::DEFAULT:
		GoToBehavior = nullptr;
		FollowBehavior = nullptr;
		DefaultBehavior = NewObject<UCameraDefaultBehavior>();
		CameraBehavior = DefaultBehavior;
		break;

	case ECameraBehavior::DYNAMIC:
		GoToBehavior = nullptr;
		FollowBehavior = nullptr;
		DynamicBehavior = NewObject<UCameraDynamicBehavior>();
		HeightBehavior = NewObject<UHeightCameraBehavior>();
		CameraBehavior = DynamicBehavior;
		break;

	case ECameraBehavior::GOTO:
		DynamicBehavior = nullptr;
		DefaultBehavior = nullptr;
		HeightBehavior = nullptr;
		FollowBehavior = nullptr;
		GoToBehavior = NewObject<UGoToBehavior>();
		CameraBehavior = GoToBehavior;
		break;

	case ECameraBehavior::LEADER:
		LeaderBehavior = NewObject<UCameraLeaderBehavior>();
		CameraBehavior = LeaderBehavior;
		break;

	case ECameraBehavior::FOLLOW_PATH:
		FollowBehavior = NewObject<UCameraFollowBehavior>();
		GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Blue, TEXT("His Follow"));

		CameraBehavior = FollowBehavior;
		break;

	case ECameraBehavior::NO_BEHAVIOR:
	default:
		DynamicBehavior = nullptr;
		DefaultBehavior = nullptr;
		HeightBehavior = nullptr;
		GoToBehavior = nullptr;
		FollowBehavior = nullptr;
		CameraBehavior = nullptr;
		break;
	}

	if (CameraBehavior) {
		CameraBehavior->BeginBehavior(this);
	}

	if (HeightBehavior) {
		HeightBehavior->BeginBehavior(this);
	}
}

void ALuminariaCamera::OnPlayerDeath(ADiamondProjectCharacter* Character,EDeathCause DeathCause) {
	ECameraBehavior CurrentBehavior = BehaviorState;

	if (CurrentBehavior == ECameraBehavior::DEFAULT) {
		return;
	}

	//if (bHasDead) {
		//return;
	//}
	
	//bHasDead = true;

	BehaviorState = ECameraBehavior::GOTO;

	GEngine->AddOnScreenDebugMessage(-1, 5.F, FColor::Red, TEXT("[CAMERA] DeathPlayer On Camera"));

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
			GoToBehaviorComponent->NextBehavior = CurrentBehavior;
		}
	});
}
