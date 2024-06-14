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

#include "../CameraBehaviors/CameraShakeBehavior.h"

#include "../SubSystems/MecanismEventsDispatcher.h"

ALuminariaCamera::ALuminariaCamera() {
	PrimaryActorTick.bCanEverTick = true;
}

void ALuminariaCamera::BeginPlay() {
	Super::BeginPlay();

	UPlayerManager* PlayerManager = GetWorld()->GetSubsystem<UPlayerManager>();
	PlayerManager->OnPlayerRegister.AddDynamic(this, &ALuminariaCamera::OnPlayerRegister);
	PlayerManager->OnPlayerDeath.AddDynamic(this, &ALuminariaCamera::OnPlayerDeath);

	UMecanismEventsDispatcher* MecanismEventsDispatcher = GetWorld()->GetSubsystem<UMecanismEventsDispatcher>();
	MecanismEventsDispatcher->OnMecanismOn.AddDynamic(this, &ALuminariaCamera::OnMecanismOn);

	StartPosition = GetActorLocation();

	InitBehavior();

	if (CurrentArea) {
		CurrentArea->SetVisited(true);
	}

}

void ALuminariaCamera::InitBehavior() {
	SwitchBehavior(BehaviorState);
}

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

	if (ShakeBehavior) {
		//ShakeBehavior->TickBehavior(DeltaTime);
	}
}

void ALuminariaCamera::OnPlayerRegister(ADiamondProjectCharacter* Character) {
	Characters.Add(Character);
}

void ALuminariaCamera::InitCameraShake() {
	ShakeBehavior = NewObject<UCameraShakeBehavior>();

	ShakeBehavior->BeginBehavior(this);

	FTimerHandle ShakeTimer;

	GetWorld()->GetTimerManager().SetTimer(ShakeTimer, [this]() {
		ShakeBehavior = nullptr;	
	}, 3.F, false);

	
}

void ALuminariaCamera::SwitchBehavior(ECameraBehavior SwitchBehavior, TFunction<void(UCameraBehavior* AddedComponent)> ResultFunc /*= [](UCameraBehavior* CameraBehavior) {}*/) {
	SwitchBehaviorFromBlueprint(SwitchBehavior);
	ResultFunc(CameraBehavior);
}

UCameraBehavior* ALuminariaCamera::SwitchBehaviorFromBlueprint(ECameraBehavior SwitchBehavior) {
	if (CameraBehavior && BehaviorState == SwitchBehavior) {
		UE_LOG(LogTemp, Error, TEXT("The camera has already this behavior."));
		return nullptr;
	}

	//CameraBehavior = NewObject<UCameraBehavior>(Behavior); Doesn't work with child functions
	BehaviorState = SwitchBehavior;

	switch (SwitchBehavior) {
	case ECameraBehavior::DEFAULT:
		ResetAll();
		DefaultBehavior = NewObject<UCameraDefaultBehavior>();
		CameraBehavior = DefaultBehavior;
		break;

	case ECameraBehavior::DYNAMIC:
		ResetAll();
		DynamicBehavior = NewObject<UCameraDynamicBehavior>();
		HeightBehavior = NewObject<UHeightCameraBehavior>();
		CameraBehavior = DynamicBehavior;
		break;

	case ECameraBehavior::GOTO:
		ResetAll();
		GoToBehavior = NewObject<UGoToBehavior>();
		CameraBehavior = GoToBehavior;
		break;

	case ECameraBehavior::LEADER:
		ResetAll();
		LeaderBehavior = NewObject<UCameraLeaderBehavior>();
		CameraBehavior = LeaderBehavior;
		break;

	case ECameraBehavior::FOLLOW_PATH:
		ResetAll();
		FollowBehavior = NewObject<UCameraFollowBehavior>();
		CameraBehavior = FollowBehavior;
		break;

	case ECameraBehavior::NO_BEHAVIOR:
	default:
		ResetAll();
		break;
	}

	if (CameraBehavior) {
		CameraBehavior->BeginBehavior(this);
	}

	if (HeightBehavior) {
		HeightBehavior->BeginBehavior(this);
	}

	return CameraBehavior;
}

void ALuminariaCamera::ResetAll() {
	DynamicBehavior = nullptr;
	DefaultBehavior = nullptr;
	HeightBehavior = nullptr;
	GoToBehavior = nullptr;
	FollowBehavior = nullptr;
	CameraBehavior = nullptr;
	ShakeBehavior = nullptr;
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

void ALuminariaCamera::OnMecanismOn(AMecanism* Mecanism) {
}