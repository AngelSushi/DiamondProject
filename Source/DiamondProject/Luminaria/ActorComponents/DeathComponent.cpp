#include "DiamondProject/Luminaria/ActorComponents/DeathComponent.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerEventsDispatcher.h"

void UDeathComponent::BeginPlay() {
	OnComponentBeginOverlap.AddDynamic(this, &UDeathComponent::OnBeginOverlap);

	UPlayerEventsDispatcher* PlayerEventsDispatcher = GetWorld()->GetSubsystem<UPlayerEventsDispatcher>();
	PlayerEventsDispatcher->OnPlayerRegister.AddDynamic(this, &UDeathComponent::RegisterPlayer);
}

void UDeathComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor->IsA(ADiamondProjectCharacter::StaticClass())) {
		GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Red, TEXT("Death"));
		
		for (ADiamondProjectCharacter* Character : Characters) {
			Character->Death();
		}
	}

}

void UDeathComponent::RegisterPlayer(ADiamondProjectCharacter* Character) {
	Characters.Add(Character);
}
