#include "CharacterStateAttract.h"
#include "CharacterStateIdle.h"
#include "../Core/DiamondProjectCharacter.h"
#include "../SubSystems/AbsorberEventsDispatcher.h"
#include "../Actors/Absorber.h"

#include "../SubSystems/UISubsystem.h"
#include "../UMG/UIComboInput.h"
#include "../DataAssets/UIDataAsset.h"

void UCharacterStateAttract::OnStateInit() {
	AbsorberEventsDispatcher = GetCharacter()->GetWorld()->GetSubsystem<UAbsorberEventsDispatcher>();
	AbsorberEventsDispatcher->OnUnDetectPlayer.AddDynamic(this, &UCharacterStateAttract::OnUnDetectPlayer);
	AbsorberEventsDispatcher->OnDetectPlayer.AddDynamic(this, &UCharacterStateAttract::OnDetectPlayer);

	UISystem = GetCharacter()->GetWorld()->GetSubsystem<UUISubsystem>();
}

void UCharacterStateAttract::OnStateBegin() {}

void UCharacterStateAttract::OnStateTick(float DeltaTime) {
	
	if (!CurrentAbsorber) {
		return;
	}

	if (!ComboWidget) {
		ComboWidget = CreateWidget<UUIComboInput>(GetCharacter()->GetWorld(), UISystem->GetUIAsset()->ComboInputClass);
		TArray<TEnumAsByte<EInput>> Inputs;
		Inputs.Add(CurrentAbsorber->GetCurrentInput());
		ComboWidget->InitComboUI(Inputs);
		ComboWidget->AddToViewport();
	}

	GetCharacter()->AddMovementInput(FVector::RightVector,CurrentAbsorber->GetAbsorberForce());
}

void UCharacterStateAttract::OnStateExit() {
	ComboWidget->RemoveFromViewport();

	ComboWidget = nullptr;
}

void UCharacterStateAttract::OnDetectPlayer(ADiamondProjectCharacter* Character, AAbsorber* Absorber) { // Called twice but not important 
	DetectedCharacter = Character;
	CurrentAbsorber = Absorber;
}

void UCharacterStateAttract::OnUnDetectPlayer(ADiamondProjectCharacter* Character, AAbsorber* Absorber) {
	ChangeState(GetStateMachine()->StateIdle);
}

void UCharacterStateAttract::OnJump() {}