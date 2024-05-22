#include "DiamondProject/Luminaria/Actors/ElectricityOrb.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectPlayerController.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"

#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

AElectricityOrb::AElectricityOrb() {
 	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = mesh;

	_sphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	_sphereCollision->SetupAttachment(RootComponent);
	// FVector Scale, bool bAutoDestroy, bool bAutoActivate, ENCPoolMethod PoolingMethod, bool bPreCullCheck)
	
	//Particle->SetupAttachment(mesh);
	
}

void AElectricityOrb::BeginPlay() {
	Super::BeginPlay();

	Particle = UNiagaraFunctionLibrary::SpawnSystemAttached( ParticleSystem,RootComponent,
		NAME_None, FVector::Zero(), FRotator(0.f),
		EAttachLocation::Type::KeepRelativeOffset, true);
}

void AElectricityOrb::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (Sender && Receiver) {

		if (!_hasBeenSet) {
			SetActorLocation(Sender->GetPawn()->GetActorLocation());
			TargetX = Sender->GetPawn()->GetActorLocation().X - 10.F;
			_hasBeenSet = true;
		}	

		float Limit = Sender->GetPlayer()->GetLightEnergy() / 100000.F;

		GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Red, FString::Printf(TEXT("Limit %f"), Limit));

		FVector SenderPosition = Sender->GetPawn()->GetActorLocation();
		FVector ReceiverPosition = Receiver->GetPawn()->GetActorLocation();

		float MaxDistance = FVector::Distance(SenderPosition, ReceiverPosition);
		float CurrentDistance = FVector::Distance(GetActorLocation(), SenderPosition);

		float DeltaDistance = LastDistance + orbSpeed * DeltaTime;

		Alpha = DeltaDistance / MaxDistance;
		Alpha = FMath::Clamp(Alpha, 0.F, 1.F);
		
		FVector LerpPosition = FMath::Lerp(SenderPosition,ReceiverPosition,Alpha);	

		if (Alpha < Limit) {
			GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Green, TEXT("Green Color Orb"));
			Particle->SetVariableLinearColor(FName("ParticleColor"), FLinearColor(0, 1.F, 0.F, 1.F));
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Orange, TEXT("Orange Color Orb"));
			Particle->SetVariableLinearColor(FName("ParticleColor"), FLinearColor(1.F, 0.45F, 0.F, 1.F));
		}

		SetActorLocation(FVector(TargetX, LerpPosition.Y, LerpPosition.Z));

		LastDistance = CurrentDistance;
	}
}



