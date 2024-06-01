#include "CharacterStateDie.h"
#include "CharacterStateRespawn.h"

#include "../Core/DiamondProjectCharacter.h"
#include "../SubSystems/PlayerManager.h"

#include "NiagaraComponent.h"

void UCharacterStateDie::OnStateInit() {
	PlayerManager = GetCharacter()->GetWorld()->GetSubsystem<UPlayerManager>();

	PlayerManager->OnPlayerRespawn.AddDynamic(this, &UCharacterStateDie::OnPlayerRespawn);
}

void UCharacterStateDie::OnStateBegin() {
	DieTickTimer = 0.F;
}

void UCharacterStateDie::OnStateTick(float DeltaTime) {
	DieTickTimer += DeltaTime;
	float Alpha = DieTickTimer / DieTimer;

	GetCharacter()->GetMesh()->SetScalarParameterValueOnMaterials(FName("Animation"), Alpha);
	GetCharacter()->DeathRespawnParticle->SetNiagaraVariableFloat(FString("User_Animation"), Alpha);
}

void UCharacterStateDie::OnPlayerRespawn(ADiamondProjectCharacter* Character, EDeathCause DeathCause, FVector RespawnPoint) {
	ChangeState(GetStateMachine()->StateRespawn);
}
