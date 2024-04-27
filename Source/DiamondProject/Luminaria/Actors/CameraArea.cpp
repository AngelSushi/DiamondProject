#include "DiamondProject/Luminaria/Actors/CameraArea.h"
#include "Components/BoxComponent.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

ACameraArea::ACameraArea() {

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	BoxCollision->SetupAttachment(RootComponent);

	
}

void ACameraArea::BeginPlay() {
	Super::BeginPlay();
	
	FVector BoxExtent = BoxCollision->GetScaledBoxExtent();

	if (APlayerStart* PlayerStart = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()))) {
		MinPosition = FVector2D(GetActorLocation().Y - BoxExtent.Y, GetActorLocation().Z - BoxExtent.Z)  - FVector2D(1, 0) * 70.F ;
		MaxPosition = FVector2D(GetActorLocation().Y + BoxExtent.Y, GetActorLocation().Z + BoxExtent.Z)  + FVector2D(1, 0) * 70.F ;
	}


}


