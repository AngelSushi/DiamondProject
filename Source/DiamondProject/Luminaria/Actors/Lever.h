#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interface/InputDrawable.h"
#include "MecanismActivator.h"
#include "Lever.generated.h"

UCLASS()
class DIAMONDPROJECT_API ALever : public AMecanismActivator,public IInputDrawable {
	GENERATED_BODY()
	
public:	
	ALever();


	UFUNCTION(BlueprintCallable)
	virtual void EnableInputListener() override;

	UFUNCTION(BlueprintCallable)
	virtual void DisableInputListener() override;

	UFUNCTION()
	virtual void CompleteInput(UInputUI* Input) override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
