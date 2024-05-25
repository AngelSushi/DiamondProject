#include "DiamondProject/Luminaria/Actors/GrowthOrb.h"
#include "Components/PointLightComponent.h"

AGrowthOrb::AGrowthOrb() {
 	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComponent;
}

void AGrowthOrb::BeginPlay() {
	Super::BeginPlay();
}

void AGrowthOrb::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

