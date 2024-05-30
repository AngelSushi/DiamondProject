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
	DIE,
};

class UCharacterStateIdle;
class UCharacterStateMovement;
class UCharacterStateJump;
class UCharacterStateDie;
class UCharacterStateFall;
class UCharacterStateAttract;

UCLASS()
class DIAMONDPROJECT_API UCharacterStateMachine : public UObject {
	GENERATED_BODY()

public:

	UCharacterStateMachine();

	UPROPERTY()
	TEnumAsByte<ECharacterState> CharacterState;

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

	// Fonction Init ? 
private:
	UPROPERTY()
	TObjectPtr<UCharacterState> CurrentState;

	UPROPERTY()
	TObjectPtr<ADiamondProjectCharacter> Character;

public:
	void SMInit(ADiamondProjectCharacter* StateCharacter);
	void SMBegin();
	void SMTick(float DeltaTime);
	void ChangeState(UCharacterState* NewState);

	void OnMovement(const FInputActionValue& Value);
	void OnJump();
	void OnInputJumpReleased();
	void OnDie();
	void OnAbsorberDetectCharacter(ADiamondProjectCharacter* DetectedCharacter,AAbsorber* Absorber);
	void OnAbsorberInputStarted();
};
