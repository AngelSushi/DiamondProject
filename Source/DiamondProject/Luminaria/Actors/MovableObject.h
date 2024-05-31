#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interface/InputUI.h"
#include "MovableObject.generated.h"

UCLASS()
class DIAMONDPROJECT_API AMovableObject : public AActor,public IInputUI {
	GENERATED_BODY()
	
public:	
	AMovableObject();

	virtual void AddInputUI();
	virtual void RemoveInputUI();
	virtual void CompleteInput();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	TObjectPtr<class UUISubsystem> UISystem;

	UPROPERTY()
	TObjectPtr<class UInputUIManager> InputUIManager;
};
