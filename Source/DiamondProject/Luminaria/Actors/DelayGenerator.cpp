#include "DiamondProject/Luminaria/Actors/DelayGenerator.h"
#include "DiamondProject/Luminaria/SubSystems/MecanismEventsDispatcher.h"
#include "DiamondProject/Luminaria/ActorComponents/MecanismComponent.h"

ADelayGenerator::ADelayGenerator() {
}

void ADelayGenerator::BeginPlay() {
	Super::BeginPlay();

	_mecanismEventsDispatcher->OnMecanismActivate.AddDynamic(this, &ADelayGenerator::OnMecanismActivateDelay);
	_mecanismEventsDispatcher->OnMecanismDeactivate.AddDynamic(this, &ADelayGenerator::OnMecanismDeactivateDelay);
	basicMaterial = mesh->GetMaterial(0)->GetMaterial();
}

void ADelayGenerator::OnMecanismActivateDelay(AMecanism* mecanism, AMecanismActivator* mecanismActivator) {
	if (mecanismActivator == this) {
		FTimerHandle DelayTimer;

		GetWorld()->GetTimerManager().SetTimer(DelayTimer, [&]() {
			_mecanismEventsDispatcher->OnMecanismDeactivate.Broadcast(mecanism, this);
		} , Delay, false);
	}
}

void ADelayGenerator::OnMecanismDeactivateDelay(AMecanism* mecanism, AMecanismActivator* mecanismActivator) {
	if (mecanismActivator == this) {
		mesh->SetMaterial(0,basicMaterial);
	}

}
