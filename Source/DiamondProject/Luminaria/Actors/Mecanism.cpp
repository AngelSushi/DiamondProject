#include "DiamondProject/Luminaria/Actors/Mecanism.h"
#include "Components/BoxComponent.h"

#include "DiamondProject/MecanismEventsDispatcher.h"
#include "DiamondProject/Luminaria/Actors/MecanismActivator.h"

AMecanism::AMecanism() {

	sceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	RootComponent = sceneRoot;

	boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	boxCollision->SetupAttachment(RootComponent);
}

void AMecanism::BeginPlay() {
	Super::BeginPlay();

	_mecanismEventsDispatcher = GetWorld()->GetSubsystem<UMecanismEventsDispatcher>();
	
	_mecanismEventsDispatcher->OnMecanismActivate.AddDynamic(this, &AMecanism::OnMecanismActivate);
	_mecanismEventsDispatcher->OnMecanismDeactivate.AddDynamic(this, &AMecanism::OnMecanismDeactivate);
}

void AMecanism::OnMecanismActivate(AMecanism* mecanism,AMecanismActivator* mecanismActivator) { // Besoin du mécanisme 
	bool isMecanismOn = true;

	for (AMecanismActivator* mecanismActivator : mecanismActivators) {
		if (!mecanismActivator->IsActivatorActive()) {
			isMecanismOn = false;
			break;
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Yellow, FString::Printf(TEXT("Is Mecanism On %f"), isMecanismOn));

	if (isMecanismOn) {
		GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Green, TEXT("Solve Mecanism"));
		_mecanismEventsDispatcher->OnMecanismOn.Broadcast();
		_isSolve = true;
	}
}

void AMecanism::OnMecanismDeactivate(AMecanism* mecanism,AMecanismActivator* mecanismActivator) {
	_mecanismEventsDispatcher->OnMecanismOff.Broadcast();
}
