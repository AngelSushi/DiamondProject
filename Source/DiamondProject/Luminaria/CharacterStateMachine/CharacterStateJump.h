#pragma once

#include "CoreMinimal.h"
#include "CharacterStateMovement.h"
#include "CharacterStateJump.generated.h"


UCLASS()
class DIAMONDPROJECT_API UCharacterStateJump : public UCharacterStateMovement {
	GENERATED_BODY()

public:

	UFUNCTION()
	void SetJumpMinDuration(float MinDuration) { JumpMinDuration = MinDuration; }
	
	UFUNCTION()
	void SetJumpMaxDuration(float MaxDuration) { JumpMaxDuration = MaxDuration; }

	virtual void OnDie() override;
	virtual void OnAbsorberDetectCharacter(ADiamondProjectCharacter* Character, AAbsorber* Absorber);

protected:
	virtual void OnStateBegin() override;
	virtual void OnStateTick(float DeltaTime);

	virtual void OnInputJumpReleased() override;
	
private:
	UFUNCTION()
	void OnPlayerLandOnGround(ADiamondProjectCharacter* Character);

	void StopJump();

	UPROPERTY()
	float JumpTimer;

	UPROPERTY()
	bool bIsJumping;

	UPROPERTY()
	bool bIsJumpPressed;

	UPROPERTY()
	float JumpMinDuration;

	UPROPERTY()
	float JumpMaxDuration;
};
