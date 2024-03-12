#include "DiamondProject/Luminaria/Actors/OnOffGenerator.h"
#include "Components/BoxComponent.h"
#include "DiamondProject/Luminaria/Actors/ElectricityOrb.h"
#include "DiamondProject/Luminaria/SubSystems/MecanismEventsDispatcher.h"
#include "DiamondProject/Luminaria/ActorComponents/MecanismComponent.h"

AOnOffGenerator::AOnOffGenerator() {}

void AOnOffGenerator::BeginPlay() {
	Super::BeginPlay();

	_mecanismEventsDispatcher->OnMecanismDeactivate.AddDynamic(this, &AOnOffGenerator::OnMecanismDeactivateOnOff);

	boxCollision->OnComponentEndOverlap.AddDynamic(this, &AOnOffGenerator::OnEndOverlapOnOff);
	basicMaterial = mesh->GetMaterial(0)->GetMaterial();

}

void AOnOffGenerator::OnEndOverlapOnOff(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor->IsA(AElectricityOrb::StaticClass())) {
		isActivatorActivate = false;
		_mecanismEventsDispatcher->OnMecanismDeactivate.Broadcast(targetMecanism,this);
	}
}

void AOnOffGenerator::OnMecanismDeactivateOnOff(AMecanism* Mecanism, AMecanismActivator* MecanismActivator) {
	if (MecanismActivator == this) {
		mesh->SetMaterial(0, basicMaterial);
	}
}
