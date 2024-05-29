#include "CharacterStateAttract.h"
#include "CharacterStateIdle.h"
#include "../Core/DiamondProjectCharacter.h"
#include "../SubSystems/AbsorberEventsDispatcher.h"
#include "../Actors/Absorber.h"

void UCharacterStateAttract::OnStateInit() {
	AbsorberEventsDispatcher = GetCharacter()->GetWorld()->GetSubsystem<UAbsorberEventsDispatcher>();
	AbsorberEventsDispatcher->OnUnDetectPlayer.AddDynamic(this, &UCharacterStateAttract::OnUnDetectPlayer);
	AbsorberEventsDispatcher->OnDetectPlayer.AddDynamic(this, &UCharacterStateAttract::OnDetectPlayer);
}

void UCharacterStateAttract::OnStateTick(float DeltaTime) {
	GetCharacter()->AddMovementInput(FVector::RightVector,CurrentAbsorber->GetAbsorberForce());
}

void UCharacterStateAttract::OnDetectPlayer(ADiamondProjectCharacter* Character, AAbsorber* Absorber) { // Called twice but not important 
	DetectedCharacter = Character;
	CurrentAbsorber = Absorber;
}

void UCharacterStateAttract::OnUnDetectPlayer(ADiamondProjectCharacter* Character, AAbsorber* Absorber) {
	ChangeState(GetStateMachine()->StateIdle);
}

void UCharacterStateAttract::OnJump() {}