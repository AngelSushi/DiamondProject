#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MecanismActivator.generated.h"


class UMecanismEventsDispatcher;

UCLASS()
class DIAMONDPROJECT_API AMecanismActivator : public AActor {
	GENERATED_BODY()
	
public:	
	AMecanismActivator();

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> root;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UMecanismComponent> mesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBoxComponent> boxCollision;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class AMecanism> targetMecanism;


	UFUNCTION()
	bool IsActivatorActive() { return isActivatorActivate; }

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SetActivatorActivate(bool IsActive);

protected:

	UPROPERTY()
	bool isActivatorActivate;

	UPROPERTY()
	UMecanismEventsDispatcher* _mecanismEventsDispatcher;
};