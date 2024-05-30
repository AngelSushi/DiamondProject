#include "DiamondProject/Luminaria/Actors/Absorber.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerManager.h"
#include "../SubSystems/AbsorberEventsDispatcher.h"
#include "../CharacterStateMachine/CharacterStateMachine.h"
#include "../UMG/UIComboInput.h"
#include "../DataAssets/AbsorberDataAsset.h"

AAbsorber::AAbsorber() {
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

void AAbsorber::BeginPlay() {
	Super::BeginPlay();

	PlayerManager = GetWorld()->GetSubsystem<UPlayerManager>();
	AbsorberEventsDispatcher = GetWorld()->GetSubsystem<UAbsorberEventsDispatcher>();

	AbsorberEventsDispatcher->OnStunAbsorber.AddDynamic(this, &AAbsorber::OnStunAbsorber);
	AbsorberEventsDispatcher->OnDeStunAbsorber.AddDynamic(this, &AAbsorber::OnDeStunAbsorber);

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AAbsorber::OnBeginOverlap);
	if (!AbsorberAsset) {
		return;
	}

	RadiusDetection = AbsorberAsset->RadiusDetection;
	AbsorberForce = AbsorberAsset->AbsorberForce;
	PossibleInputs = AbsorberAsset->PossibleInputs;
	MaxStunTimer = AbsorberAsset->MaxStunTimer;
}

void AAbsorber::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	DrawDebugSphere(GetWorld(), GetActorLocation(), RadiusDetection, 16	, FColor::Cyan);

	if (!bIsStun) {
		if (!DetectedPlayer) {
			for (ADiamondProjectCharacter* Character : PlayerManager->GetAllCharactersRef()) {
				float Distance = FVector::DistSquared(Character->GetActorLocation(), GetActorLocation());

				if (Distance <= RadiusDetection * RadiusDetection) {
					// Check s'il y a un mur entre les deux 
					DetectedPlayer = Character;
					float DistanceSqr = FVector::DistSquared(GetActorLocation(),DetectedPlayer->GetActorLocation());

					FVector Direction = GetActorLocation() - DetectedPlayer->GetActorLocation();
					Direction.Normalize();
					TArray<FHitResult> HitResults;

					GetWorld()->LineTraceMultiByChannel(HitResults, DetectedPlayer->GetActorLocation(), DetectedPlayer->GetActorLocation() + Direction * (DistanceSqr * DistanceSqr), ECC_GameTraceChannel2);

					for (FHitResult Result : HitResults) {
						GEngine->AddOnScreenDebugMessage(-1, 10.F, FColor::Magenta, FString::Printf(TEXT("Hit Name %s"), *Result.GetActor()->GetActorNameOrLabel()));
					}

					GenerateInput();
					Character->GetStateMachine()->OnAbsorberDetectCharacter(Character, this);
					AbsorberEventsDispatcher->OnDetectPlayer.Broadcast(Character, this);
					break;
				}
			}
		}
		else {
			float Distance = FVector::DistSquared(DetectedPlayer->GetActorLocation(), GetActorLocation());

			if (Distance >= RadiusDetection * RadiusDetection) {
				AbsorberEventsDispatcher->OnUnDetectPlayer.Broadcast(DetectedPlayer, this);
				DetectedPlayer = nullptr;
				return;
			}
		}
	}
	else {
		StunTimer += DeltaTime;
		
		if (StunTimer >= MaxStunTimer) {
			AbsorberEventsDispatcher->OnDeStunAbsorber.Broadcast(this);
		}
	}

}

void AAbsorber::GenerateInput() {
	if (PossibleInputs.Num() == 0) {
		return;
	}

	int RandomInput = FMath::RandRange(0, PossibleInputs.Num() - 1);

	if (RandomInput < 0) {
		return;
	}

	CurrentInput = PossibleInputs[RandomInput];

	if (PossibleInputs.Num() > 1) {
		PossibleInputs.Remove(CurrentInput);

		if (LastInput != EInput::NO_INPUT) {
			PossibleInputs.Add(LastInput);
		}

		LastInput = CurrentInput;
	}
}

void AAbsorber::OnStunAbsorber(AAbsorber* Absorber) {
	if (Absorber == this) {
		bIsStun = true;
		StunTimer = 0.F;
		DetectedPlayer = nullptr;
	}
}

void AAbsorber::OnDeStunAbsorber(AAbsorber* Absorber) {
	if (Absorber == this) {
		bIsStun = false;
	}
}

void AAbsorber::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (ADiamondProjectCharacter* Character = Cast<ADiamondProjectCharacter>(OtherActor)) {
		for (ADiamondProjectCharacter* TargetCharacter : PlayerManager->GetAllCharactersRef()) {
			TargetCharacter->Death(EDeathCause::ABSORBER);
		}
	}
}
