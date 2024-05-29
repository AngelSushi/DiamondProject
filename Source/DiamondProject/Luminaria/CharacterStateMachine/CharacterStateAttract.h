#pragma once

#include "CoreMinimal.h"
#include "CharacterState.h"
#include "CharacterStateAttract.generated.h"

class AAbsorber;

UCLASS()
class DIAMONDPROJECT_API UCharacterStateAttract : public UCharacterState {
	GENERATED_BODY()

public:
	virtual void OnJump() override;

protected:
	virtual void OnStateInit() override;
	virtual void OnStateTick(float DeltaTime) override;

private:

	UFUNCTION()
	void OnDetectPlayer(ADiamondProjectCharacter* Character, AAbsorber* Absorber);

	UFUNCTION()
	void OnUnDetectPlayer(ADiamondProjectCharacter* Character, AAbsorber* Absorber);

	UPROPERTY()
	TObjectPtr<class UAbsorberEventsDispatcher> AbsorberEventsDispatcher;

	UPROPERTY()
	TObjectPtr<class ADiamondProjectCharacter> DetectedCharacter;

	UPROPERTY()
	TObjectPtr<class AAbsorber> CurrentAbsorber;
	
};
