#include "DiamondProject/Luminaria/Actors/ElectricDetector.h"
#include "Components/BoxComponent.h"
#include "DiamondProject/Luminaria/Actors/ElectricityOrb.h"
#include "DiamondProject/Luminaria/Actors/Link.h"
#include "DiamondProject/MecanismEventsDispatcher.h"
#include "DiamondProject/Luminaria/Actors/MecanismActivator.h"

AElectricDetector::AElectricDetector() {
	PrimaryActorTick.bCanEverTick = true;
}

void AElectricDetector::BeginPlay() {
	Super::BeginPlay();

	_mecanismEventsDispatcher = GetWorld()->GetSubsystem<UMecanismEventsDispatcher>();
	_mecanismEventsDispatcher->OnMecanismActivate.AddDynamic(this,&AElectricDetector::OnMecanismActivate);
	_mecanismEventsDispatcher->OnMecanismDeactivate.AddDynamic(this, &AElectricDetector::OnMecanismDeactivate);

	boxCollision->OnComponentBeginOverlap.AddDynamic(this, &AElectricDetector::OnBeginOverlap);
}

void AElectricDetector::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void AElectricDetector::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor->IsA(AElectricityOrb::StaticClass())) {
		AElectricityOrb* orb = Cast<AElectricityOrb>(OtherActor);

		if (orb) {
//			_mecanismEventsDispatcher->OnMecanismActivate.Broadcast(this);
		}
	}
}

void AElectricDetector::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor->IsA(ALink::StaticClass())) {
		if (shouldLinkStayOn && isActivatorActivate) {
		//	_mecanismEventsDispatcher->OnMecanismDeactivate.Broadcast(this);
		}
	}
}

void AElectricDetector::OnMecanismActivate(AMecanism* mecanism,AMecanismActivator* mecanismActivator) {
	if (mecanismActivator == this) {
		mesh->SetMaterial(0, activeMaterial);
		isActivatorActivate = true;
	}
}

void AElectricDetector::OnMecanismDeactivate(AMecanism* mecanism,AMecanismActivator* mecanismActivator) {
	if (mecanismActivator == this) {
		isActivatorActivate = false;
		// Remettre le matérial par défaut
	}
}

