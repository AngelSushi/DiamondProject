#include "InputUISpawner.h"

AInputUISpawner::AInputUISpawner() {
 	PrimaryActorTick.bCanEverTick = true;
}

void AInputUISpawner::BeginPlay() {
	Super::BeginPlay();
}

void AInputUISpawner::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

