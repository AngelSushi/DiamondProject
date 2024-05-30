#pragma once

#include "CoreMinimal.h"
#include "CharacterState.h"
#include "InputCoreTypes.h"
#include "CharacterStateAttract.generated.h"

class AAbsorber;

UCLASS()
class DIAMONDPROJECT_API UCharacterStateAttract : public UCharacterState {
	GENERATED_BODY()

public:
	virtual void OnJump() override;
	virtual void OnAbsorberInputStarted(FKey Key) override;


protected:
	virtual void OnStateInit() override;
	virtual void OnStateBegin() override;
	virtual void OnStateTick(float DeltaTime) override;
	virtual void OnStateExit() override;

private:

	UFUNCTION()
	void OnDetectPlayer(ADiamondProjectCharacter* Character, AAbsorber* Absorber);

	UFUNCTION()
	void OnUnDetectPlayer(ADiamondProjectCharacter* Character, AAbsorber* Absorber);

	UFUNCTION()
	EInput ConvertKeyToInput(FKey Key);

	UPROPERTY()
	TObjectPtr<class UAbsorberEventsDispatcher> AbsorberEventsDispatcher;

	UPROPERTY()
	TObjectPtr<class UUISubsystem> UISystem;

	UPROPERTY()
	TObjectPtr<class ADiamondProjectCharacter> DetectedCharacter;

	UPROPERTY()
	TObjectPtr<class AAbsorber> CurrentAbsorber;

	UPROPERTY()
	TObjectPtr<class UUIComboInput> ComboWidget;

	UPROPERTY()
	bool bIsSpam;

	UPROPERTY()
	float TimerCountdown;

	UPROPERTY()
	float SpamTimer;

	UPROPERTY()
	float SpamMaxTimer = 2.F;
	
};
