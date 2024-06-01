#pragma once

#include "CoreMinimal.h"
#include "CharacterStateMovement.h"
#include "CharacterStateFall.generated.h"

/**
 * 
 */
UCLASS()
class DIAMONDPROJECT_API UCharacterStateFall : public UCharacterStateMovement {
	GENERATED_BODY()

public:
	virtual void OnAbsorberDetectCharacter(ADiamondProjectCharacter* Character, AAbsorber* Absorber);

protected:
	virtual void OnStateBegin() override;
	virtual void OnStateExit() override;
};
