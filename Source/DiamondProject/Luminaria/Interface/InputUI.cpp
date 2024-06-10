#include "InputUI.h"
#include "../SubSystems/PlayerManager.h"
#include "Kismet/GameplayStatics.h"

#include "../SubSystems/InputUIManager.h"
#include "../UMG/UIComboInput.h"

void UInputUI::Register(UInputUIManager* Manager, TSubclassOf<class AActor> Class, bool IsEnabled /* = false */, int InputIndex /*= -1*/) {
	TargetClass = Class;
	bIsEnabled = IsEnabled;
	bHasCompleted = false;
	InputManager = Manager;
	Index = InputIndex;
}

void UInputUI::Init(TArray<TEnumAsByte<EInput>> TargetInputs) {
	UPlayerManager* PlayerManager = InputManager->GetWorld()->GetSubsystem<UPlayerManager>();

	for (ADiamondProjectCharacter* Character : PlayerManager->GetAllCharactersRef()) {
		
		for (EInput Input : TargetInputs) {
			FKey Key = InputManager->ConvertInputToKey(Input);

			if (Input == JOYSTICK_L || Input == JOYSTICK_R) {
				Character->InputComponent->BindAxisKey(Key, this, &UInputUI::InputAxisListener);
				AxisInput = Input; // Only Work With One Axis on InputUI
			}
			else {
				Character->InputComponent->BindKey(Key, IE_Pressed, this, &UInputUI::InputListener);
				Character->InputComponent->BindKey(Key, IE_Released, this, &UInputUI::InputReleasedListener);
			}

		}
	}

	Inputs = TargetInputs;
}

void UInputUI::InputListener(FKey Key) {
	if (!bIsEnabled || bHasCompleted) {
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Red, TEXT("Listen Input Trigger"));
	EInput Input = InputManager->ConvertKeyToInput(Key);

	if (Input != EInput::NO_INPUT) {
		if (!PressedInputs.Contains(Input)) {
			PressedInputs.Add(Input);
		}
	}
}

void UInputUI::InputAxisListener(float Value) {
	if (!bIsEnabled || bHasCompleted) {
		return;
	}

	if (Value != 0) {
		GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Orange, TEXT("Listen Input"));
		if (AxisInput != EInput::NO_INPUT) {
			if (!PressedInputs.Contains(AxisInput)) {
				GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Orange, TEXT("Add Input"));
				PressedInputs.Add(AxisInput);
			}
		}
	}
	else {
		if (PressedInputs.Contains(AxisInput)) {
			InputReleasedListener(InputManager->ConvertInputToKey(AxisInput));
		}
	}
}

void UInputUI::InputReleasedListener(FKey Key) {
	if (!bIsEnabled || bHasCompleted) {
		return;
	}


	GEngine->AddOnScreenDebugMessage(-1, 5.F, FColor::Cyan, TEXT("Input Listener"));

	EInput Input = InputManager->ConvertKeyToInput(Key);
	if (PressedInputs.Num() != Inputs.Num()) {
		if (PressedInputs.Contains(Input)) {
			PressedInputs.Remove(Input);
		}
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.F, FColor::Yellow, TEXT("Complete"));
		bHasCompleted = true;
		InputManager->OnInputComplete.Broadcast(this);
		PressedInputs.Reset();
	}
}

void UInputUI::Reset() {
	PressedInputs.Reset();
}
