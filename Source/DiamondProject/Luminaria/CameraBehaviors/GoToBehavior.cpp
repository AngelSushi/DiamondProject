#include "DiamondProject/Luminaria/CameraBehaviors/GoToBehavior.h"
#include "DiamondProject/Luminaria/Actors/LuminariaCamera.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerManager.h"

#include "DiamondProject/Luminaria/CameraBehaviors/CameraDefaultBehavior.h"
#include "DiamondProject/Luminaria/CameraBehaviors/CameraDynamicBehavior.h"
#include "DiamondProject/Luminaria/CameraBehaviors/CameraLeaderBehavior.h"
#include "DiamondProject/Luminaria/Actors/CameraArea.h"

void UGoToBehavior::BeginBehavior(ALuminariaCamera* Owner) {
	Super::BeginBehavior(Owner);

	Barycenter = OwnerActor->GetActorLocation();

	CurrentTimer = 0.F;
	MaxTimer = 2.0F;

	OwnerActor->BehaviorState = ECameraBehavior::GOTO;
}

void UGoToBehavior::TickBehavior(float DeltaTime) {
	Super::TickBehavior(DeltaTime);

	if (NextBehavior == ECameraBehavior::DEFAULT) {
		if (!Approach(Barycenter, GoTo, OwnerActor->CurrentArea->TransitionDuration * DeltaTime)) {
			OwnerActor->SetActorLocation(Barycenter);
		}
		else {
			OwnerActor->SetActorLocation(GoTo);
			OwnerActor->SwitchBehavior(NextBehavior);
		}
	}
	else if (NextBehavior == ECameraBehavior::DYNAMIC) {
		CurrentTimer += DeltaTime;

		float Percentage = CurrentTimer / MaxTimer;

		FVector Lerp = FMath::Lerp(Barycenter, GoTo, Percentage);
		
		if (Percentage < 1.0f) {
			OwnerActor->SetActorLocation(Lerp);
		}
		else {
			OwnerActor->SetActorLocation(GoTo);
			OwnerActor->SwitchBehavior(NextBehavior);
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