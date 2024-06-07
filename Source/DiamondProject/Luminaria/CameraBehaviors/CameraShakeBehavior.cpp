#include "CameraShakeBehavior.h"
#include "../Actors/LuminariaCamera.h"

void UCameraShakeBehavior::BeginBehavior(ALuminariaCamera* Owner) {
	Super::BeginBehavior(Owner);
}

void UCameraShakeBehavior::TickBehavior(float DeltaTime) {
	Super::TickBehavior(DeltaTime);

	FVector CameraPosition = OwnerActor->GetActorLocation();
	CameraPosition.Y += FMath::RandRange(-5, 5);
	CameraPosition.Z += FMath::RandRange(-5, 5);

	OwnerActor->SetActorLocation(CameraPosition);
}


