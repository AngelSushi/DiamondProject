#pragma once

#include "CoreMinimal.h"
#include "CharacterState.h"
#include "CharacterStateReplacePosition.generated.h"

/**
 * 
 */
UCLASS()
class DIAMONDPROJECT_API UCharacterStateReplacePosition : public UCharacterState {
	GENERATED_BODY()
	
public:
	virtual void OnStateBegin() override;
	virtual void OnStateTick(float DeltaTime) override;

private:
	UPROPERTY()
	float OriginX;

};
