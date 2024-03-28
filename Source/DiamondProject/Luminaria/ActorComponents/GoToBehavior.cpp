#include "DiamondProject/Luminaria/ActorComponents/GoToBehavior.h"
#include "DiamondProject/Luminaria/Actors/LuminariaCamera.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerEventsDispatcher.h"

#include "DiamondProject/Luminaria/ActorComponents/CameraDefaultBehavior.h"
#include "DiamondProject/Luminaria/ActorComponents/CameraDynamicBehavior.h"
#include "DiamondProject/Luminaria/ActorComponents/CameraLeaderBehavior.h"

UGoToBehavior::UGoToBehavior() {
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UGoToBehavior::BeginPlay() {
	Super::BeginPlay();

	_barycenter = OwnerActor->GetActorLocation();
}

void UGoToBehavior::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Approach(_barycenter, GoTo, Speed * DeltaTime)) {
		OwnerActor->SetActorLocation(_barycenter);
	}
	else {
		OwnerActor->SetActorLocation(GoTo);
		OwnerActor->CameraBehavior = NextBehavior;

		switch (NextBehavior) {
			case ECameraBehavior::DEFAULT:
				OwnerActor->AddComponent(UCameraDefaultBehavior::StaticClass(), [](UActorComponent* Component) {});
				break;

			case ECameraBehavior::DYNAMIC:
				OwnerActor->AddComponent(UCameraDynamicBehavior::StaticClass(), [](UActorComponent* Component) {});
				break;

			case ECameraBehavior::LEADER:
				OwnerActor->AddComponent(UCameraLeaderBehavior::StaticClass(), [](UActorComponent* Component) {});
				break;
		}
	}
}

bool UGoToBehavior::Approach(FVector& Current, FVector Target, float Incr) {
	FVector Normal = (Target - Current);
	float VecLength = Normal.Length();

	if (VecLength > Incr) {
		Current += (Normal / VecLength) * Incr;
		return false;
	}
	else {
		Current = Target;
		return true;
	}
}