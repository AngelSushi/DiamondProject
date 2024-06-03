#include "CharacterStateFall.h"
#include "CharacterStateAttract.h"


void UCharacterStateFall::OnStateBegin() {
	Super::OnStateBegin(); // Change Gravity If Needed
}

void UCharacterStateFall::OnStateExit() {
	Super::OnStateExit();
}

void UCharacterStateFall::OnAbsorberDetectCharacter(ADiamondProjectCharacter* Character, AAbsorber* Absorber) {
	ChangeState(GetStateMachine()->StateAttract);
}