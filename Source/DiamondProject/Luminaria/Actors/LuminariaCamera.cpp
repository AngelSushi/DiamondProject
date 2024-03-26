#include "LuminariaCamera.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerEventsDispatcher.h"
#include "DiamondProject/Luminaria/ActorComponents/GoToBehavior.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"

#include "DiamondProject/Luminaria/ActorComponents/CameraDefaultBehavior.h"
#include "DiamondProject/Luminaria/ActorComponents/CameraLeaderBehavior.h"
#include "DiamondProject/Luminaria/ActorComponents/CameraDynamicBehavior.h"
#include "DiamondProject/Luminaria/ActorComponents/GoToBehavior.h"

ALuminariaCamera::ALuminariaCamera() {
	PrimaryActorTick.bCanEverTick = true;
}

void ALuminariaCamera::BeginPlay() {
	Super::BeginPlay();

	UPlayerEventsDispatcher* EventsDispatcher = GetWorld()->GetSubsystem<UPlayerEventsDispatcher>();
	EventsDispatcher->OnPlayerRegister.AddDynamic(this, &ALuminariaCamera::OnPlayerRegister);
	EventsDispatcher->OnPlayerDeath.AddDynamic(this, &ALuminariaCamera::OnPlayerDeath);

	StartPosition = GetActorLocation();

	LastBehavior = CameraBehavior;
}

void ALuminariaCamera::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ALuminariaCamera::OnPlayerRegister(ADiamondProjectCharacter* Character) {
	Characters.Add(Character);
}

void ALuminariaCamera::AddComponent(TSubclassOf<class UCameraBehavior> Component, TFunction<void(UActorComponent* AddedComponent)> ResultFunc) {
	if (LastBehaviorComponent) {
		GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Orange,FString::Printf(TEXT("Delete Component %s"),);
		LastBehaviorComponent->DestroyComponent();
	}

	LastBehaviorComponent = AddComponentByClass(Component, false, GetActorTransform(), true);

	ResultFunc(LastBehaviorComponent);
}


void ALuminariaCamera::OnPlayerDeath(ADiamondProjectCharacter* Character) {
	ECameraBehavior CurrentBehavior = CameraBehavior;

	if (CurrentBehavior == ECameraBehavior::DEFAULT) {
		return;
	}

	if (bHasDead) {
		return;
	}
	
	bHasDead = true;

	CameraBehavior = ECameraBehavior::GOTO;

	FTimerHandle Timer;
	GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Red, FString::FromInt(CurrentBehavior));

	AddComponent(UGoToBehavior::StaticClass(), [this,CurrentBehavior](UActorComponent* Component) {
		if (UGoToBehavior* GoToBehaviorComponent = Cast<UGoToBehavior>(Component)) {
			FVector GoTo = FVector::Zero();

			for (ADiamondProjectCharacter* ACharacter : Characters) {
				GoTo += ACharacter->GetActorLocation();
			}

			GoTo /= Characters.Num();
			GoTo.X = StartPosition.X;
			GoTo.Z = StartPosition.Z;

			GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Orange, TEXT("Add Component"));

			GoToBehaviorComponent->GoTo = GoTo;
			GoToBehaviorComponent->Speed = 1000.F;
			GoToBehaviorComponent->NextBehavior = CurrentBehavior;
		}
	});
}
