#include "LuminariaCamera.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerEventsDispatcher.h"
#include "DiamondProject/Luminaria/CameraBehaviors/GoToBehavior.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"

#include "DiamondProject/Luminaria/CameraBehaviors/CameraDefaultBehavior.h"
#include "DiamondProject/Luminaria/CameraBehaviors/CameraLeaderBehavior.h"
#include "DiamondProject/Luminaria/CameraBehaviors/CameraDynamicBehavior.h"
#include "DiamondProject/Luminaria/CameraBehaviors/GoToBehavior.h"

ALuminariaCamera::ALuminariaCamera() {
	PrimaryActorTick.bCanEverTick = true;
}

void ALuminariaCamera::BeginPlay() {
	Super::BeginPlay();

	UPlayerEventsDispatcher* EventsDispatcher = GetWorld()->GetSubsystem<UPlayerEventsDispatcher>();
	EventsDispatcher->OnPlayerRegister.AddDynamic(this, &ALuminariaCamera::OnPlayerRegister);
	EventsDispatcher->OnPlayerDeath.AddDynamic(this, &ALuminariaCamera::OnPlayerDeath);

	StartPosition = GetActorLocation();

	InitBehavior();
}

void ALuminariaCamera::InitBehavior() {
	switch (BehaviorState) {
		case ECameraBehavior::DEFAULT:
			SwitchBehavior(UCameraDefaultBehavior::StaticClass());
			break;

		case ECameraBehavior::DYNAMIC:
			SwitchBehavior(UCameraDynamicBehavior::StaticClass());
			break;

		case ECameraBehavior::GOTO:
			SwitchBehavior(UGoToBehavior::StaticClass());
			break;

		case ECameraBehavior::LEADER:
			SwitchBehavior(UCameraLeaderBehavior::StaticClass());
			break;
	}
}

// Utiliser Lerp et InverseLerp pour faire le zoomMax et la distanceMax du lien

void ALuminariaCamera::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (CameraBehavior) {
		CameraBehavior->TickBehavior(DeltaTime);
	}
}

void ALuminariaCamera::OnPlayerRegister(ADiamondProjectCharacter* Character) {
	Characters.Add(Character);
}

void ALuminariaCamera::SwitchBehavior(TSubclassOf<class UCameraBehavior> Behavior, TFunction<void(UCameraBehavior* AddedComponent)> ResultFunc /*= [](UCameraBehavior* CameraBehavior) {}*/) {

	if (CameraBehavior && CameraBehavior->GetClass() == Behavior) {
		UE_LOG(LogTemp, Error, TEXT("The camera has already this behavior."));
		return;
	}

	//CameraBehavior = NewObject<UCameraBehavior>(Behavior); Doesn't work with child functions

	switch (BehaviorState) {
		case ECameraBehavior::DEFAULT:
			DefaultBehavior = NewObject<UCameraDefaultBehavior>(Behavior);
			CameraBehavior = DefaultBehavior;
			break;

		case ECameraBehavior::DYNAMIC:
			DynamicBehavior = NewObject<UCameraDynamicBehavior>(Behavior);
			CameraBehavior = DynamicBehavior;
			break;

		case ECameraBehavior::GOTO:
			GoToBehavior = NewObject<UGoToBehavior>(Behavior);
			CameraBehavior = GoToBehavior;
			break;

		case ECameraBehavior::LEADER:
			LeaderBehavior = NewObject<UCameraLeaderBehavior>(Behavior);
			CameraBehavior = LeaderBehavior;
			break;
	}

	CameraBehavior->BeginBehavior(this);
	ResultFunc(CameraBehavior);
}

void ALuminariaCamera::OnPlayerDeath(ADiamondProjectCharacter* Character) {
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
	
	SwitchBehavior(UGoToBehavior::StaticClass(), [this,CurrentBehavior](UCameraBehavior* Component) {
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
