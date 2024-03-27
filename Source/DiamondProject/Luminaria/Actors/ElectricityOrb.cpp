#include "DiamondProject/Luminaria/Actors/ElectricityOrb.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectPlayerController.h"

#include "Components/SphereComponent.h"

AElectricityOrb::AElectricityOrb() {
 	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = mesh;

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

		FVector SenderPosition = orbSender->GetPawn()->GetActorLocation();
		FVector ReceiverPosition = orbReceiver->GetPawn()->GetActorLocation();

		float MaxDistance = FVector::Distance(SenderPosition, ReceiverPosition);
		float CurrentDistance = FVector::Distance(GetActorLocation(), SenderPosition);

		float DeltaDistance = LastDistance + orbSpeed * DeltaTime;

		Alpha = DeltaDistance / MaxDistance;
		Alpha = FMath::Clamp(Alpha, 0.F, 1.F);
		
		FVector LerpPosition = FMath::Lerp(SenderPosition,ReceiverPosition,Alpha);	

		SetActorLocation(FVector(GetActorLocation().X,LerpPosition.Y,LerpPosition.Z));

		LastDistance = CurrentDistance;
	}
}



