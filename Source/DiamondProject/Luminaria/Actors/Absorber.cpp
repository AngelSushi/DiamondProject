#include "DiamondProject/Luminaria/Actors/Absorber.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerManager.h"
#include "../SubSystems/AbsorberEventsDispatcher.h"
#include "../CharacterStateMachine/CharacterStateMachine.h"
#include "../UMG/UIComboInput.h"

AAbsorber::AAbsorber() {
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

void AAbsorber::BeginPlay() {
	Super::BeginPlay();

	PlayerManager = GetWorld()->GetSubsystem<UPlayerManager>();
	AbsorberEventsDispatcher = GetWorld()->GetSubsystem<UAbsorberEventsDispatcher>();
}

void AAbsorber::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	DrawDebugSphere(GetWorld(), GetActorLocation(), RadiusDetection, 16	, FColor::Cyan);

	if (!DetectedPlayer) {
		for (ADiamondProjectCharacter* Character : PlayerManager->GetAllCharactersRef()) {
			float Distance = FVector::DistSquared(Character->GetActorLocation(), GetActorLocation());

			if (Distance <= RadiusDetection * RadiusDetection) {
				GenerateInput();
				Character->GetStateMachine()->OnAbsorberDetectCharacter(Character, this);
				DetectedPlayer = Character;
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

void AAbsorber::GenerateInput() {
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
