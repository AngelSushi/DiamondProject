#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "../Interface/InputUI.h"
#include "InputUIManager.generated.h"


UCLASS()
class DIAMONDPROJECT_API UInputUIManager : public UWorldSubsystem {
	GENERATED_BODY()
	
public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	void OnPlayerMove();

	void CompleteInput(IInputUI* InputUI);

private:
	UPROPERTY()
	TArray<AActor*> ActorsInputUI;

	UPROPERTY()
	TArray<AActor*> ActivatedActors;

	UPROPERTY()
	TArray<IInputUI*> CompletedInputs;

	UPROPERTY()
	TObjectPtr<class UPlayerManager> PlayerManager;
};
