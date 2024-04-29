#include "DiamondProject/Luminaria/CameraBehaviors/GoToBehavior.h"
#include "DiamondProject/Luminaria/Actors/LuminariaCamera.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerManager.h"

#include "DiamondProject/Luminaria/CameraBehaviors/CameraDefaultBehavior.h"
#include "DiamondProject/Luminaria/CameraBehaviors/CameraDynamicBehavior.h"
#include "DiamondProject/Luminaria/CameraBehaviors/CameraLeaderBehavior.h"


void UGoToBehavior::BeginBehavior(ALuminariaCamera* Owner) {
	Super::BeginBehavior(Owner);

	Barycenter = OwnerActor->GetActorLocation();
}

void UGoToBehavior::TickBehavior(float DeltaTime) {
	Super::TickBehavior(DeltaTime);

	if (!Approach(Barycenter, GoTo, Speed * DeltaTime)) {
		OwnerActor->SetActorLocation(Barycenter);
	}
	else {
		OwnerActor->SetActorLocation(GoTo);
		OwnerActor->BehaviorState = NextBehavior;

		OwnerActor->SwitchBehavior(NextBehavior);
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