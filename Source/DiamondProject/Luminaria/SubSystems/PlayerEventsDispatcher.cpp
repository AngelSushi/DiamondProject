#include "PlayerEventsDispatcher.h"

void UPlayerEventsDispatcher::RegisterPlayer(ADiamondProjectCharacter* Character) {
	Characters.Add(Character);
	OnPlayerRegister.Broadcast(Character);
}
