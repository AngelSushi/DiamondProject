	#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterStateMachine.generated.h"


UENUM(BlueprintType)
enum ECharacterState {
	NO_STATE,
	IDLE,
	MOVING,
	JUMPING,
	FALLING,
	ATTRACT, // For Absorbers
	NO_INPUT,
};

UCLASS()
class DIAMONDPROJECT_API UCharacterStateMachine : public UObject {
	GENERATED_BODY()

	UCharacterStateMachine();

public:
	UPROPERTY()
	TEnumAsByte<ECharacterState> CharacterState;

	UFUNCTION()
	UCharacterState* GetCurrentState() { return CurrentState; }

	// Fonction Init ? 
private:
	UPROPERTY()
	TObjectPtr<class UCharacterState> CurrentState;

public:
	UFUNCTION()
	void BeginStateMachine();

	UFUNCTION()
	void StateTick(float DeltaTime);

	UFUNCTION()
	void SwitchState(UCharacterState* NewState);
	
};
