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

protected:
	virtual void OnStateBegin() override;
	virtual void OnStateExit() override;
};
