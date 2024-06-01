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
}

void ALink::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (_characters.Num() == 2) {
		CalculateBarycenter();
		float distance = FVector::Distance(_characters[0]->GetActorLocation(), _characters[1]->GetActorLocation());
		
		SetActorLocation(_barycenter);
		SetActorScale3D(FVector(distance / 100,0.05f,0.1f));

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
	if (FMath::IsNearlyEqual(GetActorScale3D().X, DistanceMax / 100, 1.F)) {
		
		// Pas sur du fonctionnement a 100%

		FVector NextPosition = Character->GetActorLocation() + Direction * Character->GetCharacterMovement()->GetMaxSpeed();
		AActor* OtherPlayer = PlayerManager->GetOtherPlayer(Character);

		float NewDistance = FVector::Distance(NextPosition, OtherPlayer->GetActorLocation());
	
		if (NewDistance >= DistanceMax) {
			IsCanceled = true;
		}
		else {
			IsCanceled = false;
		}
	}
}
