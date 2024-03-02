#include "DiamondProject/Luminaria/Actors/ElectricityOrb.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectPlayerController.h"

#include "Components/SphereComponent.h"
AElectricityOrb::AElectricityOrb() {
 	PrimaryActorTick.bCanEverTick = true;

	_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = _mesh;

	_sphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	_sphereCollision->SetupAttachment(RootComponent);


}

void AElectricityOrb::BeginPlay() {
	Super::BeginPlay();
}

void AElectricityOrb::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (orbSender && orbReceiver) {

		if (!_hasBeenSet) {
			SetActorLocation(orbSender->GetPawn()->GetActorLocation());
			_hasBeenSet = true;
		}

		FVector direction = orbSender->GetPawn()->GetActorLocation() - orbReceiver->GetPawn()->GetActorLocation();
		direction.Normalize();

		SetActorLocation(GetActorLocation() - direction * orbSpeed * DeltaTime);
	}
}



