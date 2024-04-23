#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mecanism.generated.h"

class AMecanismActivator;
class AMecanismRewardActor;
class UMecanismResultAction;
class UMecanismEventsDispatcher;

UCLASS()
class DIAMONDPROJECT_API AMecanism : public AActor {
	GENERATED_BODY()
	
public:	
	AMecanism();

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> sceneRoot;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBoxComponent> boxCollision;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<AMecanismActivator*> MecanismActivators;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<AMecanismRewardActor*> MecanismResults;

	UFUNCTION()
	void OnMecanismActivate(AMecanism* mecanism,AMecanismActivator* mecanismActivator);

	UFUNCTION()
	void OnMecanismDeactivate(AMecanism* mecanism,AMecanismActivator* mecanismActivator);

	UFUNCTION()
	void OnMecanismOn(AMecanism* TargetMecanism);

	UFUNCTION()
	void OnMecanismOff(AMecanism* TargetMecanism);

	UFUNCTION()
	bool IsSolve() { return _isSolve; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UMecanismEventsDispatcher* _mecanismEventsDispatcher;

	UPROPERTY()
	bool _isSolve;

};
