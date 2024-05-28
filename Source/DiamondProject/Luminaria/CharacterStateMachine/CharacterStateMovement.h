#pragma once

#include "CoreMinimal.h"
#include "CharacterState.h"
#include "CharacterStateMovement.generated.h"

UCLASS()
class DIAMONDPROJECT_API UCharacterStateMovement : public UCharacterState {
	GENERATED_BODY()

public: 
	virtual void OnMovement(const FInputActionValue& Value) override;
	virtual void OnJump() override;
	virtual void OnDie() override;
	
protected:
	virtual void OnStateInit() override;
	virtual void OnStateBegin() override;


	UPROPERTY()
	TObjectPtr<class UPlayerManager> PlayerManager;

private:

	UPROPERTY()
	TObjectPtr<class ADiamondProjectPlayerController> Controller;

	UPROPERTY()
	bool bCanFlip;

	UPROPERTY()
	FVector MovementDirection;

};
