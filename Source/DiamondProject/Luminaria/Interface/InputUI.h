#pragma once

#include "CoreMinimal.h"
#include "../UMG/UIComboInput.h"
#include "InputUI.generated.h"

class UInputUIManager;
enum EInput;

UCLASS()
class DIAMONDPROJECT_API UInputUI : public UObject {
	GENERATED_BODY()

public:

	void Register(UInputUIManager* Manager, TSubclassOf<class AActor> Class, bool IsEnabled = false, int InputIndex = -1);
	void Init(TArray<TEnumAsByte<EInput>> TargetInputs);

	void InputListener(FKey Key);
	void InputAxisListener(float Value);
	void InputReleasedListener(FKey Key);
	void Reset();

	void SetEnabled(bool bEnabled) { bIsEnabled = bEnabled; }

	bool IsEnabled() { return bIsEnabled; }
	bool HasCompleted() { return bHasCompleted; }
	TSubclassOf<class AActor> GetClass() { return TargetClass; }
	int GetIndex() { return Index; }

private:

	UPROPERTY()
	bool bHasCompleted;

	UPROPERTY()
	bool bIsEnabled;

	UPROPERTY()
	TSubclassOf<class AActor> TargetClass;

	UPROPERTY()
	TObjectPtr<class UInputUIManager> InputManager;

	UPROPERTY()
	TArray<TEnumAsByte<EInput>> Inputs;

	UPROPERTY()
	TArray<TEnumAsByte<EInput>> PressedInputs;
	
	UPROPERTY()
	int Index;

	UPROPERTY()
	TEnumAsByte<EInput> AxisInput;
};
