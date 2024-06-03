#include "CharacterStateRespawn.h"
#include "CharacterStateIdle.h"

#include "../Core/DiamondProjectCharacter.h"

#include "NiagaraComponent.h"

void UCharacterStateRespawn::OnStateBegin() {
	RespawnTickTimer = RespawnTimer;
}

void UCharacterStateRespawn::OnStateTick(float DeltaTime) {
	RespawnTickTimer -= DeltaTime;
	float Alpha = RespawnTickTimer / RespawnTimer;

	GetCharacter()->GetMesh()->SetScalarParameterValueOnMaterials(FName("Animation"), Alpha);
	GetCharacter()->DeathRespawnParticle->SetNiagaraVariableFloat(FString("User_Animation"), Alpha);

	if (RespawnTickTimer <= 0.F) {
		GetStateMachine()->ChangeState(GetStateMachine()->StateIdle);
	}

}
