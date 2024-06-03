#include "InputUI.h"
#include "../SubSystems/PlayerManager.h"
#include "Kismet/GameplayStatics.h"

#include "../SubSystems/InputUIManager.h"
#include "../UMG/UIComboInput.h"

void UInputUI::Register(UInputUIManager* Manager, TSubclassOf<class AActor> Class, bool IsEnabled /* = false */) {
	TargetClass = Class;
	bIsEnabled = IsEnabled;
	bHasCompleted = false;
	InputManager = Manager;
}

void UInputUI::Init(TArray<TEnumAsByte<EInput>> TargetInputs) {
	UPlayerManager* PlayerManager = InputManager->GetWorld()->GetSubsystem<UPlayerManager>();

	for (ADiamondProjectCharacter* Character : PlayerManager->GetAllCharactersRef()) {
		
		for (EInput Input : TargetInputs) {
			FKey Key = InputManager->ConvertInputToKey(Input);

			Character->InputComponent->BindKey(Key, IE_Pressed, this, &UInputUI::InputListener);
			Character->InputComponent->BindKey(Key, IE_Released, this, &UInputUI::InputReleasedListener);
		}
	}

	Inputs = TargetInputs;
}

void UInputUI::InputListener(FKey Key) {
	if (!bIsEnabled || bHasCompleted) {
		return;
	}

	EInput Input = InputManager->ConvertKeyToInput(Key);

	if (Input != EInput::NO_INPUT) {
		if (!PressedInputs.Contains(Input)) {
			PressedInputs.Add(Input);
		}
	}
}

void UInputUI::InputReleasedListener(FKey Key) {
	if (!bIsEnabled || bHasCompleted) {
		return;
	}

	EInput Input = InputManager->ConvertKeyToInput(Key);
	if (PressedInputs.Num() != Inputs.Num()) {
		if (PressedInputs.Contains(Input)) {
			PressedInputs.Remove(Input);
		}
	}
	else {
		bHasCompleted = true;
		InputManager->OnInputComplete.Broadcast(this);
		PressedInputs.Reset();
	}
}
