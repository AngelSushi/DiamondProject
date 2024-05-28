#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterState.generated.h"

enum ECharacterState;

UCLASS()
class DIAMONDPROJECT_API UCharacterState : public UObject {
	GENERATED_BODY()

	UCharacterState();

public:
	UPROPERTY()
	TEnumAsByte<ECharacterState> State;

	UFUNCTION()
	void StateBegin();

	UFUNCTION()
	void StateTick(float DeltaTime);

	UFUNCTION()
	void StateExit();
	
};
