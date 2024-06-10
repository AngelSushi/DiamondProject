#include "DiamondProject/Luminaria/Actors/Link.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerManager.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectPlayerController.h"

#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "NiagaraComponent.h"

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

	//ParticleSystem->AddRelativeRotation(FRotator(90, 0, 0));
}

void ALink::RegisterPlayer(ADiamondProjectCharacter* character) {
	_characters.Add(character);
	PlayersLastDirection.Add(character, FVector::Zero());
}

void ALink::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (_characters.Num() == 2) {
		CalculateBarycenter();
		float distance = FVector::Distance(_characters[0]->GetActorLocation(), _characters[1]->GetActorLocation());
		float ScaleZ = FMath::Lerp(CrushMin, CrushMax, DistanceAlpha);
		
		//GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Magenta, FString::Printf(TEXT("DistanceAlpha %f"), DistanceAlpha));
		//GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Cyan, FString::Printf(TEXT("ScaleZ %f"), ScaleZ));

		SetActorLocation(_barycenter);
		SetActorScale3D(FVector(distance / 100,0.05f, ScaleZ));

		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(_characters[0]->GetActorLocation(),_characters[1]->GetActorLocation());
		SetActorRotation(Rotation);

		//ParticleSystem->SetWorldLocation(FVector(0,GetActorScale3D().X / 2.F,0));
		//ParticleSystem->AddRelativeRotation(FRotator(90, 0, 0));
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
	float Distance = FVector::Distance(Character->GetActorLocation(), OtherPlayer->GetActorLocation());
	DistanceAlpha = Distance / DistanceMax;

	FVector LastDirection = PlayersLastDirection[Character];

	FVector OtherPlayerPosition = OtherPlayer->GetActorLocation();

	float FirstOrderPlayerIndex = PlayerManager->GetOrderedPlayers()[0];

	if (Distance >= DistanceMax) {
		if (Character == PlayerManager->GetAllCharactersRef()[FirstOrderPlayerIndex]) {
			if (Direction != FVector(0, 1, 0)) {
				IsCanceled = true;
			}
		}
		else {
			if (Direction != FVector(0, -1, 0)) {
				IsCanceled = true;
			}
		}
	}
}
