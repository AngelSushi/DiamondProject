#include "DiamondProject/Luminaria/Actors/Link.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerManager.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"

#include "Kismet/KismetMathLibrary.h"

ALink::ALink() {
	PrimaryActorTick.bCanEverTick = true;

	_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = _mesh;
}



void ALink::BeginPlay() {
	Super::BeginPlay();

	UPlayerManager* PlayerManager = GetWorld()->GetSubsystem<UPlayerManager>();
	PlayerManager->OnPlayerRegister.AddDynamic(this, &ALink::RegisterPlayer);
}

void ALink::RegisterPlayer(ADiamondProjectCharacter* character) {
	_characters.Add(character);
}

void ALink::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (_characters.Num() == 2) {
		CalculateBarycenter();
		float distance = FVector::Distance(_characters[0]->GetActorLocation(), _characters[1]->GetActorLocation());
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