#include "CharacterStateDie.h"
#include "CharacterStateRespawn.h"

#include "../Core/DiamondProjectCharacter.h"
#include "../SubSystems/PlayerManager.h"

#include "NiagaraComponent.h"

#include "Kismet/GameplayStatics.h"
#include "../Actors/Link.h"

void UCharacterStateDie::OnStateInit() {
	Super::OnStateInit();

	//PlayerManager = GetCharacter()->GetWorld()->GetSubsystem<UPlayerManager>();
		
	PlayerManager->OnPlayerRespawn.AddDynamic(this, &UCharacterStateDie::OnPlayerRespawn);
	LinkRef = Cast<ALink>(UGameplayStatics::GetActorOfClass(GetCharacter()->GetWorld(), ALink::StaticClass()));
}

void UCharacterStateDie::OnStateBegin() {
	Super::OnStateBegin();

	DieTickTimer = 0.F;
}

void UCharacterStateDie::OnStateTick(float DeltaTime) {
	Super::OnStateTick(DeltaTime);
	
	DieTickTimer += DeltaTime;
	float Alpha = DieTickTimer / DieTimer;

	GetCharacter()->GetMesh()->SetScalarParameterValueOnMaterials(FName("Animation"), Alpha);
	GetCharacter()->DeathRespawnParticle->SetNiagaraVariableFloat(FString("User_Animation"), Alpha);

	if (LinkRef) {
		LinkRef->_mesh->SetScalarParameterValueOnMaterials(FName("Animation"), (Alpha + 0.2F));
		LinkRef->DeathParticleSystem->SetNiagaraVariableFloat(FString("User_Animation"), (Alpha + 0.2F));
	}

}

void UCharacterStateDie::OnPlayerRespawn(ADiamondProjectCharacter* Character, EDeathCause DeathCause, FVector RespawnPoint) {
	ChangeState(GetStateMachine()->StateRespawn);
}
