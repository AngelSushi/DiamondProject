#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interface/InputDrawable.h"
#include "../Interface/ButtonInteractable.h"
#include "MovableObject.generated.h"

class UInputUI;

UCLASS()
class DIAMONDPROJECT_API AMovableObject : public AActor, public IInputDrawable,public IButtonInteractable {
	GENERATED_BODY()
	
public:	
	AMovableObject();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/* IInputDrawable Functions */
	UFUNCTION(BlueprintCallable)
	virtual void EnableInputListener();

	UFUNCTION(BlueprintCallable)
	virtual void DisableInputListener();

	UFUNCTION()
	virtual void CompleteInput(UInputUI* Input);

};
