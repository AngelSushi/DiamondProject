#include "DiamondProject/Luminaria/Actors/KillingShadow.h"
#include "DiamondProject/Luminaria/ActorComponents/DeathComponent.h"


// Sets default values
AKillingShadow::AKillingShadow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DeathComponent = CreateDefaultSubobject<UDeathComponent>(TEXT("Death Component"));
	DeathComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AKillingShadow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKillingShadow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

