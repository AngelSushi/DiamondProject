#include "DiamondProject/Luminaria/Actors/Spike.h"
#include "DiamondProject/Luminaria/ActorComponents/DeathComponent.h"

ASpike::ASpike() {
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	DeathComponent = CreateDefaultSubobject<UDeathComponent>(TEXT("Death Component"));
	DeathComponent->SetupAttachment(Mesh);
}

void ASpike::BeginPlay() {
	Super::BeginPlay();
	
}


