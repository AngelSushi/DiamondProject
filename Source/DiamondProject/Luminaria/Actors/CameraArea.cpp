#include "DiamondProject/Luminaria/Actors/CameraArea.h"
#include "Components/BoxComponent.h"

ACameraArea::ACameraArea() {

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	BoxCollision->SetupAttachment(RootComponent);
}

void ACameraArea::BeginPlay() {
	Super::BeginPlay();
	
	FVector BoxExtent = BoxCollision->GetScaledBoxExtent();
	MinPosition = FVector2D(GetActorLocation().X - BoxExtent.X, GetActorLocation().Z - BoxExtent.Z);
	MaxPosition = FVector2D(GetActorLocation().X + BoxExtent.X, GetActorLocation().Z + BoxExtent.Z);

}


