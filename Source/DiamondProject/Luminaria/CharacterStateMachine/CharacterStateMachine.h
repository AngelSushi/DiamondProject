	#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InputActionValue.h"
#include "CharacterStateMachine.generated.h"

class UCharacterStateIdle;
class UCharacterStateMovement;
class UCharacterStateJump;
class UCharacterStateDie;
class UCharacterStateFall;
class UCharacterStateAttract;
class UCharacterStateRespawn;
class UCharacterStateReplacePosition;
class AAbsorber;

UCLASS()
class DIAMONDPROJECT_API UCharacterStateMachine : public UObject {
	GENERATED_BODY()

public:

	UCharacterStateMachine();

	UFUNCTION()
	UCharacterState* GetCurrentState() const { return CurrentState; }

	UFUNCTION()
	ADiamondProjectCharacter* GetCharacter() const { return Character; }

	UPROPERTY() TObjectPtr<UCharacterStateIdle> StateIdle;
	UPROPERTY() TObjectPtr<UCharacterStateMovement> StateMovement;
	UPROPERTY() TObjectPtr<UCharacterStateJump> StateJump;
	UPROPERTY() TObjectPtr<UCharacterStateDie> StateDie;
	UPROPERTY() TObjectPtr<UCharacterStateFall> StateFall;
	UPROPERTY() TObjectPtr<UCharacterStateAttract> StateAttract;
	UPROPERTY() TObjectPtr<UCharacterStateRespawn> StateRespawn;
	UPROPERTY() TObjectPtr<UCharacterStateReplacePosition> StateReplace;

	// Fonction Init ? 
private:
	UPROPERTY()
	TObjectPtr<UCharacterState> CurrentState;

	UPROPERTY()
	TObjectPtr<ADiamondProjectCharacter> Character;

	UPROPERTY()
	bool bForceReplacePosition;

	UPROPERTY()
	bool bLastForceReplacePosition;

public:
	void SMInit(ADiamondProjectCharacter* StateCharacter);
	void SMBegin();
	void SMTick(float DeltaTime);
	void ChangeState(UCharacterState* NewState);

	void OnMovement(const FInputActionValue& MovementValue);
	void OnJump();
	void OnInputJumpReleased();
	void OnDie();
	void OnAbsorberDetectCharacter(ADiamondProjectCharacter* DetectedCharacter,AAbsorber* Absorber);
	void OnAbsorberInputStarted(FKey Key);
};
