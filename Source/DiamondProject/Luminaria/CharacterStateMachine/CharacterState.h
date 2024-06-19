#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterStateMachine.h"
#include "CharacterState.generated.h"

class UCharacterStateMachine;
class ADiamondProjectCharacter;

UCLASS()
class DIAMONDPROJECT_API UCharacterState : public UObject {
	GENERATED_BODY()

public:

	void StateInit(UCharacterStateMachine* CharacterStateMachine);
	
	void StateBegin();

	void StateTick(float DeltaTime);
	void StateExit();


	virtual void OnMovement(const FInputActionValue& MovementValue);
	virtual void OnJump();
	virtual void OnInputJumpReleased();
	virtual void OnDie();
	virtual void OnAbsorberDetectCharacter(ADiamondProjectCharacter* Character, AAbsorber* Absorber);
	virtual void OnAbsorberInputStarted(FKey Key);

protected:
	virtual void OnStateInit();
	virtual void OnStateBegin();
	virtual void OnStateTick(float DeltaTime);
	virtual void OnStateExit();

	void ChangeState(UCharacterState* NewState) const;

	ADiamondProjectCharacter* GetCharacter() const { return StateMachine->GetCharacter(); }

	UCharacterStateMachine* GetStateMachine() const { return StateMachine; }

	UPROPERTY()
	TObjectPtr<class UPlayerManager> PlayerManager;

private:
	UPROPERTY()
	TObjectPtr<class UCharacterStateMachine> StateMachine;

	UPROPERTY()
	FVector LastPosition;
	
};
