#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MecanismActivator.h"
#include "DefaultGenerator.generated.h"

class UMecanismEventsDispatcher;
class AMecanismActivator;

UCLASS()
class DIAMONDPROJECT_API ADefaultGenerator : public AMecanismActivator {
	GENERATED_BODY()
	
public:	
	ADefaultGenerator();


	UPROPERTY(EditAnywhere)
	TObjectPtr<class UMaterial> activeMaterial;

protected:

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);

	UFUNCTION()
	void OnMecanismActivate(AMecanism* mecanism,AMecanismActivator* mecanismActivator);

	UFUNCTION()
	void OnMecanismDeactivate(AMecanism* mecanism,AMecanismActivator* mecanismActivator);


protected:
	UPROPERTY()
	UMecanismEventsDispatcher* _mecanismEventsDispatcher;


	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
