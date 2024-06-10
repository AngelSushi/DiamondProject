#include "PlayerManager.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectPlayerController.h"

void UPlayerManager::RegisterPlayer(ADiamondProjectCharacter* Character) {
	Characters.Add(Character);
	OnPlayerRegister.Broadcast(Character);
}

ADiamondProjectCharacter* UPlayerManager::GetOtherPlayer(ADiamondProjectCharacter* Character) {
	if (Characters.Num() < 2) {
		UE_LOG(LogTemp, Error, TEXT("There is not enough players on the scene."));
		return nullptr;
	}

	return Character == Characters[0] ? Characters[1] : Characters[0];
}

void UPlayerManager::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (Characters.Num() >= 2) {

		FVector FirstPlayerPosition = Characters[0]->GetActorLocation();
		FVector SecondPlayerPosition = Characters[1]->GetActorLocation();

		if (FirstPlayerPosition.Y > SecondPlayerPosition.Y) { // The First Player Is More Left Than The Second
			if (OrderedPlayers.Num() == 0) {
				OrderedPlayers.Add(1);
				OrderedPlayers.Add(0);
			}
			else {
				if (OrderedPlayers[0] == 0) {
					OrderedPlayers.Swap(0, 1);
				}
			}
		}
		else {
			if (OrderedPlayers.Num() == 0) {
				OrderedPlayers.Add(0);
				OrderedPlayers.Add(1);
			}
			else {
				if (OrderedPlayers[0] == 1) {
					OrderedPlayers.Swap(0, 1);
				}
			}
		}
	}
}
