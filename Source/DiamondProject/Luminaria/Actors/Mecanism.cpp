#include "DiamondProject/Luminaria/Actors/Mecanism.h"
#include "Components/BoxComponent.h"

#include "DiamondProject/Luminaria/SubSystems/MecanismEventsDispatcher.h"
#include "DiamondProject/Luminaria/Actors/MecanismActivator.h"
#include "DiamondProject/Luminaria/Actors/MecanismRewardActor.h"

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

	_mecanismEventsDispatcher->OnMecanismOn.AddDynamic(this, &AMecanism::OnMecanismOn);
	_mecanismEventsDispatcher->OnMecanismOff.AddDynamic(this, &AMecanism::OnMecanismOff);
}

void AMecanism::OnMecanismActivate(AMecanism* mecanism,AMecanismActivator* mActivator) { // Besoin du mécanisme 
	if (mecanism != this) {
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Yellow, FString::Printf(TEXT("Mecanism Name %s"), *mecanism->GetActorNameOrLabel()));
	GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Green, FString::Printf(TEXT("This Name %s"), *GetActorNameOrLabel()));

	bool isMecanismOn = true;

	if (MecanismActivators.Num() == 0) {
		isMecanismOn = false;
	}

	for (AMecanismActivator* mecanismActivator : MecanismActivators) {
		if (!mecanismActivator->IsActivatorActive()) {
			isMecanismOn = false;
			break;
		}
	}

	if (isMecanismOn && !_isSolve) {
		GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Green, TEXT("Solve Mecanism"));
		_mecanismEventsDispatcher->OnMecanismOn.Broadcast(mecanism);
		_isSolve = true;
	}
}

void AMecanism::OnMecanismDeactivate(AMecanism* mecanism,AMecanismActivator* mecanismActivator) {
	_mecanismEventsDispatcher->OnMecanismOff.Broadcast(mecanism);
	_isSolve = false;
}

void AMecanism::OnMecanismOn(AMecanism* TargetMecanism) { 
	if (TargetMecanism != this) {
		return;
	}

	for (AMecanismRewardActor* MecanismRewardActor : MecanismResults) {
		if (MecanismRewardActor) {
			MecanismRewardActor->Reward();
		}
	}
}

void AMecanism::OnMecanismOff(AMecanism* TargetMecanism) {
	if (TargetMecanism != this) {
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Magenta, FString::Printf(TEXT("[CPP] [Mecanism] OnMecanismOff From %s"), *TargetMecanism->GetActorNameOrLabel()));

	for (AMecanismRewardActor* MecanismRewardActor : MecanismResults) {		
		if (!MecanismRewardActor) {
			return;
		}

		MecanismRewardActor->CancelReward();
	}
}
