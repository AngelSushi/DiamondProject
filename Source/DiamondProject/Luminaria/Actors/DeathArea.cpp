#include "DiamondProject/Luminaria/Actors/DeathArea.h"
#include "Components/BoxComponent.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerEventsDispatcher.h"

ADeathArea::ADeathArea() {
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	RootComponent = BoxCollision;
}

void ADeathArea::BeginPlay() {
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ADeathArea::OnBeginOverlap);

	UPlayerEventsDispatcher* PlayerEventsDispatcher = GetWorld()->GetSubsystem<UPlayerEventsDispatcher>();
	PlayerEventsDispatcher->OnPlayerRegister.AddDynamic(this, &ADeathArea::RegisterPlayer);
}

void ADeathArea::RegisterPlayer(ADiamondProjectCharacter* Character) {
	Characters.Add(Character);
}

void ADeathArea::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	if (OtherActor->IsA(ADiamondProjectCharacter::StaticClass())) {
		for (ADiamondProjectCharacter* Character : Characters) {
			Character->Death();
		}
	}
}

