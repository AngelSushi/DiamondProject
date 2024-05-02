#include "DiamondProject/Luminaria/Actors/CameraArea.h"
#include "Components/BoxComponent.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "DiamondProject/Luminaria/SubSystems/PlayerManager.h"
ACameraArea::ACameraArea() {

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	BoxCollision->SetupAttachment(RootComponent);

	GoTo = CreateDefaultSubobject<USceneComponent>(TEXT("GoTo"));
	GoTo->SetupAttachment(RootComponent);
}

void ACameraArea::TickArea(float DeltaTime) {

	for (ADiamondProjectCharacter* Character : PlayerManager->Characters) {
		if (Character->GetCharacterMovement()->GetMaxSpeed() != PlayerSpeed) {
			Character->GetCharacterMovement()->MaxWalkSpeed = PlayerSpeed;
		}
	}
}

void ACameraArea::BeginPlay() {
	Super::BeginPlay();
	
	FVector BoxExtent = BoxCollision->GetScaledBoxExtent();

	if (APlayerStart* PlayerStart = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()))) {
		MinPosition = FVector2D(GetActorLocation().Y - BoxExtent.Y, GetActorLocation().Z - BoxExtent.Z)  - FVector2D(1, 0) * 70.F ;
		MaxPosition = FVector2D(GetActorLocation().Y + BoxExtent.Y, GetActorLocation().Z + BoxExtent.Z)  + FVector2D(1, 0) * 70.F ;
	}

	PlayerManager = GetWorld()->GetSubsystem<UPlayerManager>();
}


