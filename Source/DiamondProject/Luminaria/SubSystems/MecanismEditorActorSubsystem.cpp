#include "DiamondProject/Luminaria/SubSystems/MecanismEditorActorSubsystem.h"
#include "DiamondProject/Luminaria/Actors/MecanismActivator.h"

void UMecanismEditorActorSubsystem::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);

	GLog->Log("Init Actor Editor Subsystem");

	OnNewActorsPlaced.AddDynamic(this, &UMecanismEditorActorSubsystem::OnPlaceActors);
}

void UMecanismEditorActorSubsystem::Deinitialize() {}

void UMecanismEditorActorSubsystem::OnPlaceActors(UObject* usedObject,const TArray<AActor*>& placedActors) {
	/*GLog->Log("Place Actors");

	for (AActor* actor : placedActors) {
		if (AMecanismActivator* mecanismActivator = Cast<AMecanismActivator>(actor)) {
			DrawDebugSphere(actor->GetWorld(),actor->GetActorLocation(),5.F,20,FColor::Red,0,15000.F);
		}
	}
	*/
}

