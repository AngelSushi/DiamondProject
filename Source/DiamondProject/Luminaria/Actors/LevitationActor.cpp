#include "LevitationActor.h"
#include "DiamondProject/Luminaria/SubSystems/MathUtilities.h"

ALevitationActor::ALevitationActor() {
 	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComponent;
}


void ALevitationActor::BeginPlay() {
	Super::BeginPlay();

	OriginPosition = GetActorLocation();

	LevitationPosition += GetActorLocation();
	OriginSpeed = LevitationSpeed;
}

void ALevitationActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	LevitationTimer += DeltaTime * LevitationSpeed;

	bool IsPing = false;
	FVector Position = UMathUtilities::PingPongVecAscending(LevitationTimer, LevitationPosition, OriginPosition,IsPing);

	if (!IsPing) {
		LevitationSpeed = OriginSpeed * 0.9F;
	}
	else {
		LevitationSpeed = OriginSpeed;
	}

	SetActorLocation(Position);
}

