#include "CharacterStateDie.h"
#include "CharacterStateIdle.h"

#include "../Core/DiamondProjectCharacter.h"
#include "../SubSystems/PlayerManager.h"

void UCharacterStateDie::OnStateInit() {
	PlayerManager = GetCharacter()->GetWorld()->GetSubsystem<UPlayerManager>();

	PlayerManager->OnPlayerRespawn.AddDynamic(this, &UCharacterStateDie::OnPlayerRespawn);
}

void UCharacterStateDie::OnPlayerRespawn(ADiamondProjectCharacter* Character, EDeathCause DeathCause, FVector RespawnPoint) {
	ChangeState(GetStateMachine()->StateIdle);
}
