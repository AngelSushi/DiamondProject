#include "DeathSmoke.h"

#include "NiagaraComponent.h"
#include "../ActorComponents/DeathComponent.h"

ADeathSmoke::ADeathSmoke() {
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	NiagaraSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraSystem->SetupAttachment(Root);

	DeathComponent = CreateDefaultSubobject<UDeathComponent>(TEXT("Death"));
	DeathComponent->SetupAttachment(Root);
}

void ADeathSmoke::BeginPlay() {
	Super::BeginPlay();
	
}


