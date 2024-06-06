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
	TObjectPtr<class UMaterial> CrystalMaterial;

protected:

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnMecanismActivate(AMecanism* mecanism,AMecanismActivator* mecanismActivator);

	UFUNCTION()
	void OnMecanismDeactivate(AMecanism* mecanism,AMecanismActivator* mecanismActivator);

	UPROPERTY(EditAnywhere)
	bool bNeedLink;

	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY()
	TObjectPtr<class UMaterial> basicMaterial;

private:
	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> InstanceCrystalMaterial;

	UPROPERTY()
	float CrystalTimer;

	UPROPERTY()
	float MaxCrystalTimer = 1.F;
};
