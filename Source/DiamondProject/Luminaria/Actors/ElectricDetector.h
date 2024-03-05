#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MecanismActivator.h"
#include "ElectricDetector.generated.h"

class UMecanismEventsDispatcher;
class AMecanismActivator;

UCLASS()
class DIAMONDPROJECT_API AElectricDetector : public AMecanismActivator {
	GENERATED_BODY()
	
public:	
	AElectricDetector();

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> mesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBoxComponent> boxCollision;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UMaterial> activeMaterial;

private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere)
	bool shouldLinkStayOn;

	UPROPERTY()
	UMecanismEventsDispatcher* _mecanismEventsDispatcher;

	UFUNCTION()
	void OnMecanismActivate(AMecanism* mecanism,AMecanismActivator* mecanismActivator);

	UFUNCTION()
	void OnMecanismDeactivate(AMecanism* mecanism,AMecanismActivator* mecanismActivator);


protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
