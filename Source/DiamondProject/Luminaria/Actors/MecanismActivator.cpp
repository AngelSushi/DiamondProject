#include "DiamondProject/Luminaria/Actors/MecanismActivator.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Engine/GameViewportClient.h"

#include "Components/SplineComponent.h"
#include "ProceduralMeshComponent.h"
#include "Components/BoxComponent.h"

#include "DiamondProject/Luminaria/ActorComponents/MecanismComponent.h"
#include "DiamondProject/Luminaria/Actors/Mecanism.h"
#include "DiamondProject/Luminaria/SubSystems/MecanismEventsDispatcher.h"

AMecanismActivator::AMecanismActivator() {
 	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = root;

	mesh = CreateDefaultSubobject<UMecanismComponent>(TEXT("Mecanism"));
	mesh->SetupAttachment(root);

	boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	boxCollision->SetupAttachment(RootComponent);
}

void AMecanismActivator::BeginPlay() {
	Super::BeginPlay();

	_mecanismEventsDispatcher = GetWorld()->GetSubsystem<UMecanismEventsDispatcher>();


	TArray<AActor*> MecanismArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMecanism::StaticClass(), MecanismArray);

	for (AActor* MecanismActor : MecanismArray) {
		if (AMecanism* Mecanism = Cast<AMecanism>(MecanismActor)) {
			if (Mecanism->MecanismActivators.Contains(this)) {
				targetMecanism = Mecanism;
				break;
			}
		}
	}
}

void AMecanismActivator::SetActivatorActivate(bool IsActive) {
	isActivatorActivate = IsActive;

	if (IsActive) {
		_mecanismEventsDispatcher->OnMecanismActivate.Broadcast(targetMecanism, this);
	}
	else {
		_mecanismEventsDispatcher->OnMecanismDeactivate.Broadcast(targetMecanism, this);
	}

}

void AMecanismActivator::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}


