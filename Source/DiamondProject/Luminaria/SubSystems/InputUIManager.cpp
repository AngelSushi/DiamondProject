#include "InputUIManager.h"
#include "PlayerManager.h"
#include "../Actors/MecanismActivator.h"

#include "Kismet/GameplayStatics.h"
#include "../Actors/Link.h"
#include "../UMG/UIComboInput.h"
#include "../Interface/InputDrawable.h"
#include "../Actors/MovableObject.h"

UInputUIManager::UInputUIManager() {
	InputMovableObject = CreateDefaultSubobject<UInputUI>(TEXT("MovableObjectInput"));
	InputJump = CreateDefaultSubobject<UInputUI>(TEXT("JumpInput"));
}

void UInputUIManager::PostInitialize() {
	Super::PostInitialize();

	AddInput(InputMovableObject, AMovableObject::StaticClass(),false);
	AddInput(InputJump, ADiamondProjectCharacter::StaticClass(), false);

	FTimerHandle Timer;

	GetWorld()->GetTimerManager().SetTimer(Timer, [this]() { // Wait Player To Register	
		InputMovableObject->Init({EInput::RB,EInput::JOYSTICK_L});
		InputJump->Init({ EInput::Y });
	}, 0.3f, false);
}

void UInputUIManager::AddInput(UInputUI* Input, TSubclassOf<class AActor> Class, bool IsEnabled /* = false*/) {
	Input->Register(this, Class, IsEnabled);
	
	if (!Inputs.Contains(Input)) {
		Inputs.Add(Input);
	}
}

void UInputUIManager::EnableInput(TSubclassOf<class AActor> Class) {
	for (UInputUI* Input : Inputs) {
		if (Input->GetClass()->GetName().Equals(Class->GetName())) {
			Input->SetEnabled(true);
			break;
		}
	}
}

void UInputUIManager::DisableInput(TSubclassOf<class AActor> Class) {
	for (UInputUI* Input : Inputs) {
		if (Input->GetClass() == Class) {
			Input->SetEnabled(false);
			break;
		}
	}
}

EInput UInputUIManager::ConvertKeyToInput(FKey Key) {
	if (Key == EKeys::Gamepad_FaceButton_Bottom) {
		return EInput::A;
	}

	if (Key == EKeys::Gamepad_FaceButton_Right) {
		return EInput::B;
	}

	if (Key == EKeys::Gamepad_FaceButton_Left) {
		return EInput::X;
	}

	if (Key == EKeys::Gamepad_FaceButton_Top) {
		return EInput::Y;
	}

	if (Key == EKeys::Gamepad_LeftThumbstick) {
		return EInput::JOYSTICK_L;
	}

	if (Key == EKeys::Gamepad_RightThumbstick) {
		return EInput::JOYSTICK_R;
	}

	if (Key == EKeys::Gamepad_RightShoulder) {
		return EInput::RB;
	}

	if (Key == EKeys::Gamepad_RightTriggerAxis) {
		return EInput::RT;
	}

	if (Key == EKeys::Gamepad_LeftShoulder) {
		return EInput::LB;
	}

	if (Key == EKeys::Gamepad_LeftTriggerAxis) {
		return EInput::LT;
	}

	return EInput::NO_INPUT;

}

FKey UInputUIManager::ConvertInputToKey(EInput Input) { // Doesn't work with keyboard
	switch (Input) {
		case EInput::A:
			return EKeys::Gamepad_FaceButton_Bottom;
			break;

		case EInput::B:
			return EKeys::Gamepad_FaceButton_Right;
			break;

		case EInput::Y:
			return EKeys::Gamepad_FaceButton_Top;
			break;

		case EInput::X:
			return EKeys::Gamepad_FaceButton_Left;
			break;

		case EInput::JOYSTICK_L:
			return EKeys::Gamepad_LeftThumbstick;
			break;
			
		case EInput::JOYSTICK_R:
			return EKeys::Gamepad_RightThumbstick;
			break;

		case EInput::RB:
			return EKeys::Gamepad_RightShoulder;
			break;

		case EInput::RT:
			return EKeys::Gamepad_RightTriggerAxis;
			break;

		case EInput::LB:
			return EKeys::Gamepad_LeftShoulder;
			break;

		case EInput::LT:
			return EKeys::Gamepad_LeftTriggerAxis;
			break;

		default:
			return EKeys::Zero;
			break;
	}
}
