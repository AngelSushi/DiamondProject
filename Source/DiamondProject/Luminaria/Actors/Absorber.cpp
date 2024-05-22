#include "DiamondProject/Luminaria/Actors/Absorber.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerManager.h"

AAbsorber::AAbsorber() {
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

void AAbsorber::BeginPlay() {
	Super::BeginPlay();

	PlayerManager = GetWorld()->GetSubsystem<UPlayerManager>();
}

void AAbsorber::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	DrawDebugSphere(GetWorld(), GetActorLocation(), RadiusDetection, 16	, FColor::Red);

	for (ADiamondProjectCharacter* Character : PlayerManager->GetAllCharactersRef()) {
		float Distance = FVector::Distance(Character->GetActorLocation(), GetActorLocation());

		if (Distance <= RadiusDetection) {

		}
	}
}

