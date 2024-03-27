#include "DiamondProject/Luminaria/ActorComponents/DeathComponent.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerEventsDispatcher.h"

void UDeathComponent::BeginPlay() {
	OnComponentBeginOverlap.AddDynamic(this, &UDeathComponent::OnBeginOverlap);

	PlayerEventsDispatcher = GetWorld()->GetSubsystem<UPlayerEventsDispatcher>();
	PlayerEventsDispatcher->OnPlayerRegister.AddDynamic(this, &UDeathComponent::RegisterPlayer);
}

void UDeathComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (ADiamondProjectCharacter* Character = Cast<ADiamondProjectCharacter>(OtherActor)) {
		GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Red, TEXT("Death"));

		for (ADiamondProjectCharacter* C : Characters) {
			C->Death();
		}
	}

}

void UDeathComponent::RegisterPlayer(ADiamondProjectCharacter* Character) {
	Characters.Add(Character);
}
