#include "CharacterStateRespawn.h"
#include "CharacterStateIdle.h"

#include "../Core/DiamondProjectCharacter.h"

#include "NiagaraComponent.h"

#include "Kismet/GameplayStatics.h"
#include "../Actors/Link.h"

void UCharacterStateRespawn::OnStateInit() {
	Super::OnStateInit();
	LinkRef = Cast<ALink>(UGameplayStatics::GetActorOfClass(GetCharacter()->GetWorld(), ALink::StaticClass()));
}

void UCharacterStateRespawn::OnStateBegin() {
	Super::OnStateBegin();
	RespawnTickTimer = RespawnTimer;
}

void UCharacterStateRespawn::OnStateTick(float DeltaTime) {
	Super::OnStateTick(DeltaTime);

	RespawnTickTimer -= DeltaTime;
	float Alpha = RespawnTickTimer / RespawnTimer;

	GetCharacter()->GetMesh()->SetScalarParameterValueOnMaterials(FName("Animation"), Alpha);
	GetCharacter()->DeathRespawnParticle->SetNiagaraVariableFloat(FString("User_Animation"), Alpha);

	if (LinkRef) {
		LinkRef->_mesh->SetScalarParameterValueOnMaterials(FName("Animation"), Alpha);
		LinkRef->DeathParticleSystem->SetNiagaraVariableFloat(FString("User_Animation"), Alpha);
	}

	if (RespawnTickTimer <= 0.F) {
		GetStateMachine()->ChangeState(GetStateMachine()->StateIdle);
	}

}
