#include "PlayerManager.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"

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
