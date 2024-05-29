#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterStateMachine.h"
#include "CharacterState.generated.h"

enum ECharacterState;
class UCharacterStateMachine;
class ADiamondProjectCharacter;

UCLASS()
class DIAMONDPROJECT_API UCharacterState : public UObject {
	GENERATED_BODY()

public:

	UPROPERTY()
	TEnumAsByte<ECharacterState> State;

	void StateInit(UCharacterStateMachine* CharacterStateMachine);
	
	void StateBegin();

	void StateTick(float DeltaTime);
	void StateExit();


	virtual void OnMovement(const FInputActionValue& Value);
	virtual void OnJump();
	virtual void OnInputJumpReleased();
	virtual void OnDie();
	virtual void OnAbsorberDetectCharacter(ADiamondProjectCharacter* Character, AAbsorber* Absorber);

protected:
	virtual void OnStateInit();
	virtual void OnStateBegin();
	virtual void OnStateTick(float DeltaTime);
	virtual void OnStateExit();

	void ChangeState(UCharacterState* NewState) const;

	ADiamondProjectCharacter* GetCharacter() const { return StateMachine->GetCharacter(); }

	UCharacterStateMachine* GetStateMachine() const { return StateMachine; }
private:
	UPROPERTY()
	TObjectPtr<class UCharacterStateMachine> StateMachine;
	
};
