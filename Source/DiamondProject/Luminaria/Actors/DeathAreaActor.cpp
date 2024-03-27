#include "DiamondProject/Luminaria/Actors/DeathAreaActor.h"
#include "Components/BoxComponent.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerEventsDispatcher.h"
#include "DiamondProject/Luminaria/ActorComponents/DeathComponent.h"

ADeathAreaActor::ADeathAreaActor() {
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	DeathComponent = CreateDefaultSubobject<UDeathComponent>(TEXT("Death Component"));
	DeathComponent->SetupAttachment(RootComponent);
}

void ADeathAreaActor::BeginPlay() {
	Super::BeginPlay();
}

