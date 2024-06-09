#include "InputUIManager.h"
#include "PlayerManager.h"
#include "../Actors/MecanismActivator.h"

#include "Kismet/GameplayStatics.h"
#include "../Actors/Link.h"
#include "../UMG/UIComboInput.h"
#include "../Interface/InputDrawable.h"
#include "../Actors/MovableObject.h"
#include "../Actors/Lever.h"

UInputUIManager::UInputUIManager() {
	InputMovableObject = CreateDefaultSubobject<UInputUI>(TEXT("MovableObjectInput"));
	InputJump = CreateDefaultSubobject<UInputUI>(TEXT("JumpInput"));
	InputMegaJump = CreateDefaultSubobject<UInputUI>(TEXT("MegaJumpInput"));
	InputLever = CreateDefaultSubobject<UInputUI>(TEXT("LeverInput"));
	InputTransfer = CreateDefaultSubobject<UInputUI>(TEXT("TransferInput"));
}

void UInputUIManager::PostInitialize() {
	Super::PostInitialize();

	AddInput(InputMovableObject, AMovableObject::StaticClass(),false);
	AddInput(InputJump, ADiamondProjectCharacter::StaticClass(), false,0);
	AddInput(InputMegaJump, ADiamondProjectCharacter::StaticClass(), false, 1);
	AddInput(InputLever, ALever::StaticClass(), false);
	AddInput(InputTransfer, ALink::StaticClass(), false);

	FTimerHandle Timer;

	GetWorld()->GetTimerManager().SetTimer(Timer, [this]() { // Wait Player To Register	
		InputMovableObject->Init({EInput::RT,EInput::JOYSTICK_L});
		InputJump->Init({ EInput::Y });
		InputMegaJump->Init({ EInput::Y });
		InputLever->Init({ EInput::X });
		InputLever->Init({ EInput::B });
	}, 0.3f, false);
}

void UInputUIManager::AddInput(UInputUI* Input, TSubclassOf<class AActor> Class, bool IsEnabled /* = false*/, int InputIndex /*= -1*/) {
	Input->Register(this, Class, IsEnabled,InputIndex);
	
	if (!Inputs.Contains(Input)) {
		Inputs.Add(Input);
	}
}

void UInputUIManager::EnableInput(TSubclassOf<class AActor> Class, int Index /*= -1*/) {
	for (UInputUI* Input : Inputs) {
		if (Input->GetClass()->GetName().Equals(Class->GetName())) {
			if (Index != -1) {
				if (Index == Input->GetIndex()) {
					Input->SetEnabled(true);
					break;
				}
			}
			else {
				Input->SetEnabled(true);
				break;
			}
		}
	}
}

void UInputUIManager::DisableInput(TSubclassOf<class AActor> Class, int Index /*= -1*/) {
	for (UInputUI* Input : Inputs) {
		if (Input->GetClass() == Class) { // Check if True
			if (Index != -1) {
				if (Index == Input->GetIndex()) {
					Input->SetEnabled(false);
					Input->Reset();
					break;
				}
			}
			else {
				Input->SetEnabled(false);
				Input->Reset();
				break;
			}
		}
	}
}

UInputUI* UInputUIManager::GetInputWithClass(TSubclassOf<class AActor> Class) {
	for (UInputUI* Input : Inputs) {
		if (Input->GetClass() == Class) {
			return Input;
		}
	}

	return nullptr;
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

	if (Key == EKeys::Gamepad_LeftX) {
		return EInput::JOYSTICK_L;
	}

	if (Key == EKeys::Gamepad_RightX) { // Gamepad_RightThumbstick
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
			return EKeys::Gamepad_LeftX; // LeftThumbstick
			break;
			
		case EInput::JOYSTICK_R:
			return EKeys::Gamepad_RightX; // RightTHumbstick
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
