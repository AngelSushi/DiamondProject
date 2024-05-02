#include "DiamondProject/Luminaria/Actors/Link.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerManager.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectPlayerController.h"

#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

ALink::ALink() {
	PrimaryActorTick.bCanEverTick = true;

	_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = _mesh;
}

void ALink::BeginPlay() {
	Super::BeginPlay();

	PlayerManager = GetWorld()->GetSubsystem<UPlayerManager>();
	PlayerManager->OnPlayerRegister.AddDynamic(this, &ALink::RegisterPlayer);
	PlayerManager->OnPlayerMove.AddDynamic(this, &ALink::OnPlayerMove);
}

void ALink::RegisterPlayer(ADiamondProjectCharacter* character) {
	_characters.Add(character);
}

void ALink::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (_characters.Num() == 2) {
		CalculateBarycenter();
		float distance = FVector::Distance(_characters[0]->GetActorLocation(), _characters[1]->GetActorLocation());
		distance = FMath::Clamp(distance, 0, DistanceMax);

		SetActorLocation(_barycenter);
		SetActorScale3D(FVector(distance / 100,0.2f,0.1f));

		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(_characters[0]->GetActorLocation(),_characters[1]->GetActorLocation());
		SetActorRotation(Rotation);
	}

}

void ALink::CalculateBarycenter() {
	FVector First = _characters[0]->GetActorLocation();
	FVector Second = _characters[1]->GetActorLocation();

	float divider = 2.F;

	if (!_characters[0]->GetMesh()->IsVisible()) {
		First = FVector::Zero();
		divider -= 1.F;
	}

	if (!_characters[1]->GetMesh()->IsVisible()) {
		Second = FVector::Zero();
		divider -= 1.F;
	}

	_barycenter = (First + Second) / divider;
}

void ALink::OnPlayerMove(ADiamondProjectCharacter* Character, FVector Direction, bool& IsCanceled) {
	if (FMath::IsNearlyEqual(GetActorScale3D().X, DistanceMax / 100, 1.F)) {
		
		FVector NextPosition = Character->GetActorLocation() + Direction * Character->GetCharacterMovement()->GetMaxSpeed();
		AActor* OtherPlayer = PlayerManager->GetOtherPlayer(Character);

		float NewDistance = FVector::Distance(NextPosition, OtherPlayer->GetActorLocation());
	
		if (NewDistance >= DistanceMax) {
			IsCanceled = true;

			if (ADiamondProjectPlayerController* PlayerController = Cast<ADiamondProjectPlayerController>(Character->GetController())) {
				if (PlayerController->bIsJumping) {			
					Character->GetCharacterMovement()->GravityScale = 15.0F;
					PlayerController->bIsJumping = false;
				}
			}
		}
		else {
			IsCanceled = false;
		}
	}
}
