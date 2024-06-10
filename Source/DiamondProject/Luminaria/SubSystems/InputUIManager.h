#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "../Interface/InputUI.h"
#include "InputUIManager.generated.h"

enum EInput;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputComplete, UInputUI*, Input);


UCLASS()
class DIAMONDPROJECT_API UInputUIManager : public UWorldSubsystem {
	GENERATED_BODY()
	
public:
	UInputUIManager();

	virtual void PostInitialize() override;

	UFUNCTION()
	void AddInput(UInputUI* Input, TSubclassOf<class AActor> Class, bool IsEnabled = false,int InputIndex = -1);

	UFUNCTION()
	void EnableInput(TSubclassOf<class AActor> Class,int Index = -1);

	UFUNCTION()
	void DisableInput(TSubclassOf<class AActor> Class,int Index = -1);

	UFUNCTION()
	UInputUI* GetInputWithClass(TSubclassOf<class AActor> Class);

	UFUNCTION()
	static EInput ConvertKeyToInput(FKey Key);

	UFUNCTION()
	static FKey ConvertInputToKey(EInput Input);

	// Events
	UPROPERTY(VisibleAnywhere)
	FOnInputComplete OnInputComplete;


private:
	UPROPERTY() TObjectPtr<class UInputUI> InputMovableObject;
	UPROPERTY() TObjectPtr<class UInputUI> InputJump;
	UPROPERTY() TObjectPtr<class UInputUI> InputMegaJump;
	UPROPERTY() TObjectPtr<class UInputUI> InputLever;
	UPROPERTY() TObjectPtr<class UInputUI> InputTransfer;

	UPROPERTY()
	TArray<UInputUI*> Inputs;

	UPROPERTY()
	TArray<AActor*> ActivatedActors;

};
