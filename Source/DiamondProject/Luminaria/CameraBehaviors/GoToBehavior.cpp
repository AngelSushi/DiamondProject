#include "DiamondProject/Luminaria/CameraBehaviors/GoToBehavior.h"
#include "DiamondProject/Luminaria/Actors/LuminariaCamera.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerEventsDispatcher.h"

#include "DiamondProject/Luminaria/CameraBehaviors/CameraDefaultBehavior.h"
#include "DiamondProject/Luminaria/CameraBehaviors/CameraDynamicBehavior.h"
#include "DiamondProject/Luminaria/CameraBehaviors/CameraLeaderBehavior.h"


void UGoToBehavior::BeginBehavior(ALuminariaCamera* Owner) {
	Super::BeginBehavior(Owner);

	_barycenter = OwnerActor->GetActorLocation();
}

void UGoToBehavior::TickBehavior(float DeltaTime) {
	Super::TickBehavior(DeltaTime);

	if (!Approach(_barycenter, GoTo, Speed * DeltaTime)) {
		GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Red, TEXT("Try to Approach"));
		OwnerActor->SetActorLocation(_barycenter);
	}
	else {
		OwnerActor->SetActorLocation(GoTo);
		OwnerActor->BehaviorState = NextBehavior;
		GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Green, TEXT("Finish Approach"));

		switch (NextBehavior) {
			case ECameraBehavior::DEFAULT:
				OwnerActor->SwitchBehavior(UCameraDefaultBehavior::StaticClass());
				break;

			case ECameraBehavior::DYNAMIC:
				OwnerActor->SwitchBehavior(UCameraDynamicBehavior::StaticClass());
				break;

			case ECameraBehavior::LEADER:
				OwnerActor->SwitchBehavior(UCameraLeaderBehavior::StaticClass());
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