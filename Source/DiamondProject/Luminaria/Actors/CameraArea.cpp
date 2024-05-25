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

	//if (AreaBehavior == ECameraBehavior::DEFAULT && !GoTo) {
		GoTo = CreateDefaultSubobject<USceneComponent>(TEXT("GoTo"));
		GoTo->SetupAttachment(RootComponent);
//	}
}

void ACameraArea::TickArea(float DeltaTime) {
	for (ADiamondProjectCharacter* Character : PlayerManager->Characters) {
		Character->GetCharacterMovement()->MaxWalkSpeed = FMath::Clamp(Character->GetCharacterMovement()->MaxWalkSpeed,PlayerSpeedMin,PlayerSpeedMax);
	}
}

void ACameraArea::BeginPlay() {
	Super::BeginPlay();
	
	FVector BoxExtent = BoxCollision->GetScaledBoxExtent();

	if (APlayerStart* PlayerStart = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()))) {
		MinPosition = FVector2D(GetActorLocation().Y - BoxExtent.Y, GetActorLocation().Z - BoxExtent.Z)  - FVector2D(1, 0) * 70.F ;
		MaxPosition = FVector2D(GetActorLocation().Y + BoxExtent.Y, GetActorLocation().Z + BoxExtent.Z)  + FVector2D(1, 0) * 70.F ;
	}

	bHasVisited = false;
	PlayerManager = GetWorld()->GetSubsystem<UPlayerManager>();

	if (GoTo && AreaBehavior == ECameraBehavior::DEFAULT && GoTo->GetRelativeLocation() == FVector::Zero()) {
		GoTo->SetRelativeLocation(FVector(-ZoomMin, 0, 0));
	}
}

#if WITH_EDITOR
void ACameraArea::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	//Super::PostEditChangeProperty(PropertyChangedEvent);

	if (AreaBehavior == ECameraBehavior::DEFAULT) {
		ZoomMax = ZoomMin;
	}

	if (PropertyChangedEvent.GetMemberPropertyName() == "AreaBehavior") {
		switch (AreaBehavior) {
			case ECameraBehavior::DEFAULT:
				BoxCollision->ShapeColor = FColor::Cyan;
				break;

			case ECameraBehavior::DYNAMIC:
				BoxCollision->ShapeColor = FColor::Yellow;
				break;

			case ECameraBehavior::NO_BEHAVIOR:
				BoxCollision->ShapeColor = FColor::White;
				break;

			case ECameraBehavior::GOTO:
			case ECameraBehavior::LEADER:
				BoxCollision->ShapeColor = FColor::Black;
				break;
		}

		/*if (AreaBehavior == ECameraBehavior::DEFAULT) {
			if (!GoTo && GetWorld()) {
				GoTo = NewObject<USceneComponent>(this, USceneComponent::StaticClass(), TEXT("GoTo"));
				GoTo->SetupAttachment(Root);
				GoTo->RegisterComponent();
				GoTo->SetRelativeLocation(FVector(-ZoomMin, 0, 0));
			}
		}
		else {
			if (GoTo) {
				GoTo->DestroyComponent();
				GoTo = nullptr;
			}
		}*/
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif


