#include "DiamondProject/Luminaria/Actors/DefaultGenerator.h"
#include "Components/BoxComponent.h"
#include "DiamondProject/Luminaria/Actors/ElectricityOrb.h"
#include "DiamondProject/Luminaria/Actors/Link.h"
#include "DiamondProject/Luminaria/SubSystems/MecanismEventsDispatcher.h"
#include "DiamondProject/Luminaria/Actors/MecanismActivator.h"

#include "DiamondProject/Luminaria/ActorComponents/MecanismComponent.h"

ADefaultGenerator::ADefaultGenerator() {
	PrimaryActorTick.bCanEverTick = true;
}

void ADefaultGenerator::BeginPlay() {
	Super::BeginPlay();

	_mecanismEventsDispatcher->OnMecanismActivate.AddDynamic(this,&ADefaultGenerator::OnMecanismActivate);
	_mecanismEventsDispatcher->OnMecanismDeactivate.AddDynamic(this, &ADefaultGenerator::OnMecanismDeactivate);

	boxCollision->OnComponentBeginOverlap.AddDynamic(this, &ADefaultGenerator::OnBeginOverlap);
	boxCollision->OnComponentEndOverlap.AddDynamic(this, &ADefaultGenerator::OnEndOverlap);

	basicMaterial = mesh->GetMaterial(0)->GetMaterial();

	InstanceCrystalMaterial = UMaterialInstanceDynamic::Create(CrystalMaterial, this);
	mesh->SetMaterial(1, InstanceCrystalMaterial);
}

void ADefaultGenerator::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (IsActivatorActive()) {
		CrystalTimer += DeltaTime;

		float Alpha = (CrystalTimer) / MaxCrystalTimer;
		Alpha = FMath::Clamp(Alpha, 0.F, 1.0F);
		InstanceCrystalMaterial->SetScalarParameterValue("BlendAlpha", Alpha);
	}
	else {
		InstanceCrystalMaterial->SetScalarParameterValue("BlendAlpha", 0.f);
	}
}

void ADefaultGenerator::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor->IsA(AElectricityOrb::StaticClass())) {
		AElectricityOrb* orb = Cast<AElectricityOrb>(OtherActor);

		if (orb) {
			SetActivatorActivate(true);
		}
	}
}

void ADefaultGenerator::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor->IsA(ALink::StaticClass())) {
		if (bNeedLink) {
			SetActivatorActivate(false);
		}
	}

}

void ADefaultGenerator::OnMecanismActivate(AMecanism* mecanism,AMecanismActivator* mecanismActivator) {
	if (mecanismActivator == this) {
		//mesh->SetMaterial(0, activeMaterial);
		CrystalTimer = 0.F;
	}
}

void ADefaultGenerator::OnMecanismDeactivate(AMecanism* mecanism,AMecanismActivator* mecanismActivator) {
	if (mecanismActivator == this) {
		mesh->SetMaterial(0, basicMaterial);
	}
}

