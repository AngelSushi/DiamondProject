#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MecanismActivator.generated.h"




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

	UFUNCTION()
	bool IsActivatorActive() { return isActivatorActivate; }

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaTime) override;

	virtual bool ShouldTickIfViewportsOnly() const override;

protected:

	UPROPERTY()
	bool isActivatorActivate;

};