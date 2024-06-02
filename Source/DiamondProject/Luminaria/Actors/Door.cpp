#include "DiamondProject/Luminaria/Actors/Door.h"
#include "DiamondProject/Luminaria/DataAssets/DoorDataAsset.h"

ADoor::ADoor() {
	PrimaryActorTick.bCanEverTick = true;

}

void ADoor::BeginPlay() {
	Super::BeginPlay();
	
}

void ADoor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}


