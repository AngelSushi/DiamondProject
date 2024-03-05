#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MecanismActivator.generated.h"

UCLASS()
class DIAMONDPROJECT_API AMecanismActivator : public AActor {
	GENERATED_BODY()
	
public:	
	AMecanismActivator();

	UFUNCTION()
	bool IsActivatorActive() { return isActivatorActivate; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	bool isActivatorActivate;

public:
	virtual void Tick(float DeltaTime) override;

};
