#include "DiamondProject/Luminaria/Actors/Absorber.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerManager.h"
#include "../SubSystems/AbsorberEventsDispatcher.h"
#include "../CharacterStateMachine/CharacterStateMachine.h"

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
				GEngine->AddOnScreenDebugMessage(-1, 5.F, FColor::Green, TEXT("Detect Player"));
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
			GEngine->AddOnScreenDebugMessage(-1, 5.F, FColor::Red, TEXT("UnDetect Player"));
			AbsorberEventsDispatcher->OnUnDetectPlayer.Broadcast(DetectedPlayer, this);
			DetectedPlayer = nullptr;
			return;
		}
	}

}
