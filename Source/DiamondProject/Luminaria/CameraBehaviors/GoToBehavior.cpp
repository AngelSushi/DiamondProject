#include "DiamondProject/Luminaria/CameraBehaviors/GoToBehavior.h"
#include "DiamondProject/Luminaria/Actors/LuminariaCamera.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerManager.h"

#include "DiamondProject/Luminaria/CameraBehaviors/CameraDefaultBehavior.h"
#include "DiamondProject/Luminaria/CameraBehaviors/CameraDynamicBehavior.h"
#include "DiamondProject/Luminaria/CameraBehaviors/CameraLeaderBehavior.h"
#include "DiamondProject/Luminaria/Actors/CameraArea.h"

void UGoToBehavior::BeginBehavior(ALuminariaCamera* Owner) {
	Super::BeginBehavior(Owner);

	Barycenter = Owner->GetActorLocation();
	Owner->BehaviorState = ECameraBehavior::GOTO;

	CurrentTimer = 0.F;
	MaxTimer = 2.0F;
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

			FTimerHandle ReachDelay;

			OwnerActor->GetWorld()->GetTimerManager().SetTimer(ReachDelay, [this]() {
				OwnerActor->SwitchBehavior(NextBehavior);
			}, DelayAtReach, false);

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