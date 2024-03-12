#include "DiamondProject/Luminaria/Actors/DefaultGenerator.h"
#include "Components/BoxComponent.h"
#include "DiamondProject/Luminaria/Actors/ElectricityOrb.h"
#include "DiamondProject/Luminaria/Actors/Link.h"
#include "DiamondProject/Luminaria/SubSystems/MecanismEventsDispatcher.h"
#include "DiamondProject/Luminaria/Actors/MecanismActivator.h"

ADefaultGenerator::ADefaultGenerator() {
	PrimaryActorTick.bCanEverTick = true;
}

void ADefaultGenerator::BeginPlay() {
	Super::BeginPlay();

	_mecanismEventsDispatcher = GetWorld()->GetSubsystem<UMecanismEventsDispatcher>();
	_mecanismEventsDispatcher->OnMecanismActivate.AddDynamic(this,&ADefaultGenerator::OnMecanismActivate);
	_mecanismEventsDispatcher->OnMecanismDeactivate.AddDynamic(this, &ADefaultGenerator::OnMecanismDeactivate);

	boxCollision->OnComponentBeginOverlap.AddDynamic(this, &ADefaultGenerator::OnBeginOverlap);


}

void ADefaultGenerator::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void ADefaultGenerator::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor->IsA(AElectricityOrb::StaticClass())) {
		AElectricityOrb* orb = Cast<AElectricityOrb>(OtherActor);

		if (orb) {
			isActivatorActivate = true;
			_mecanismEventsDispatcher->OnMecanismActivate.Broadcast(targetMecanism,this);
		}
	}
}

void ADefaultGenerator::OnMecanismActivate(AMecanism* mecanism,AMecanismActivator* mecanismActivator) {
	if (mecanismActivator == this) {
		mesh->SetMaterial(0, activeMaterial);
	}
}

void ADefaultGenerator::OnMecanismDeactivate(AMecanism* mecanism,AMecanismActivator* mecanismActivator) {
	if (mecanismActivator == this) {
		//GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Red, TEXT("Mecanism Deactivate Parent"));
		isActivatorActivate = false;
	}
}

