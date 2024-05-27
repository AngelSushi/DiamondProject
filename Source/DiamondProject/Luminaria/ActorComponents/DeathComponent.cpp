#include "DiamondProject/Luminaria/ActorComponents/DeathComponent.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerManager.h"

UDeathComponent::UDeathComponent() {

}

void UDeathComponent::BeginPlay() {
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UDeathComponent::OnBeginOverlap);

	PlayerManager = GetWorld()->GetSubsystem<UPlayerManager>();
	PlayerManager->OnPlayerRegister.AddDynamic(this, &UDeathComponent::RegisterPlayer);
}

void UDeathComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	//GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Red, TEXT("Ask For Death "));

	if (ADiamondProjectCharacter* Character = Cast<ADiamondProjectCharacter>(OtherActor)) {
		GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Red, TEXT("Death"));

		for (ADiamondProjectCharacter* C : Characters) {
			C->Death((EDeathCause) DeathCause.GetValue());
		}
	}

}

void UDeathComponent::RegisterPlayer(ADiamondProjectCharacter* Character) {
	Characters.Add(Character);
}
