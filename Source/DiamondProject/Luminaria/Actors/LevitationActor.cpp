#include "LevitationActor.h"

ALevitationActor::ALevitationActor() {
 	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComponent;
}


void ALevitationActor::BeginPlay() {
	Super::BeginPlay();

	OriginPosition = GetActorLocation();
	bDescent = true;

	LevitationPosition += GetActorLocation();
	CalculateSpeed = LevitationSpeed;
}

void ALevitationActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	SetActorLocation(PingPong(OriginPosition, LevitationPosition, CalculateSpeed * DeltaTime));
}

FVector ALevitationActor::PingPong(FVector& Start, FVector& End, float Speed) {
	FVector Current = GetActorLocation();
	FVector Target = bDescent ? End : Start;
	
	FVector Normal = (Target - Current);
	float Length = Normal.Length();

	if (Length > Speed) {
		Current += (Normal / Length) * Speed;
	}
	else {
		Current = Target;
		bDescent = !bDescent;
		CalculateSpeed = bDescent ? LevitationSpeed : LevitationSpeed / 1.75f;
	}
	
	return Current;


}

