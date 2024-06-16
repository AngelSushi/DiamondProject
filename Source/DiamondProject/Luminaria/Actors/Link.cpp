#include "DiamondProject/Luminaria/Actors/Link.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerManager.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectPlayerController.h"

#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "NiagaraComponent.h"

#include "CameraArea.h"
#include "../DataAssets/CameraAreaDataAsset.h"

ALink::ALink() {
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	_mesh->SetupAttachment(RootComponent);

	DeathParticleSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Death"));
	DeathParticleSystem->SetupAttachment(_mesh);
}

void ALink::BeginPlay() {
	Super::BeginPlay();

	PlayerManager = GetWorld()->GetSubsystem<UPlayerManager>();
	PlayerManager->OnPlayerRegister.AddDynamic(this, &ALink::RegisterPlayer);
	PlayerManager->OnPlayerMove.AddDynamic(this, &ALink::OnPlayerMove);

	PlayerManager->OnChangeNewArea.AddDynamic(this, &ALink::OnChangeNewArea);

	//ParticleSystem->AddRelativeRotation(FRotator(90, 0, 0));
}

void ALink::OnChangeNewArea(ACameraArea* NewArea) {
	DistanceMax = NewArea->GetDataAsset()->LinkMaxDistance;
}

void ALink::RegisterPlayer(ADiamondProjectCharacter* character) {
	_characters.Add(character);
}

void ALink::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (_characters.Num() == 2) {
		CalculateBarycenter();
		float distance = FVector::Distance(_characters[0]->GetActorLocation(), _characters[1]->GetActorLocation());
		float ScaleZ = FMath::Lerp(CrushMin, CrushMax, DistanceAlpha);
		
		SetActorLocation(_barycenter);
		SetActorScale3D(FVector(distance / 100,0.05f, ScaleZ));

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

void ALink::OnPlayerMove(ADiamondProjectCharacter* Character,FVector2D Input,FVector Direction,bool& IsCanceled) {
	AActor* OtherPlayer = PlayerManager->GetOtherPlayer(Character);

	float FirstOrderPlayerIndex = PlayerManager->GetOrderedPlayers()[0];

	float DistanceY = FMath::Abs(Character->GetActorLocation().Y - OtherPlayer->GetActorLocation().Y);
	float DistanceZ = FMath::Abs(Character->GetActorLocation().Z - OtherPlayer->GetActorLocation().Z);

	float Distance = FVector(1, DistanceY, DistanceZ).Length();

	DistanceAlpha = Distance / DistanceMax;

	if (DistanceY >= DistanceMax) { // Celui qui est le plus a gauche peut bouger si DistanceZ >= DistanceMax ( A verifier si pas trop chiant niveau gamefeel)
		if (Character == PlayerManager->GetAllCharactersRef()[FirstOrderPlayerIndex]) {
			if (Direction != FVector(0, 1, 0)) {
				IsCanceled = true;
			}
		}
		else {
			if (DistanceZ >= DistanceMax || Direction != FVector(0, -1, 0)) {
				IsCanceled = true;
			}
		}
	}
	else {
		IsCanceled = false;
	}
}
