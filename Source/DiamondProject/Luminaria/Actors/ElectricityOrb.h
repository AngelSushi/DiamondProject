#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElectricityOrb.generated.h"

UCLASS()
class DIAMONDPROJECT_API AElectricityOrb : public AActor {
	GENERATED_BODY()
	
private:

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USphereComponent> _sphereCollision;

	UPROPERTY()
	bool _hasBeenSet;

	UPROPERTY()
	float LastDistance;
	
	UPROPERTY()
	float Alpha;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UNiagaraSystem> ParticleSystem;

	UPROPERTY()
	TObjectPtr<class UNiagaraComponent> Particle;

public:	
	AElectricityOrb();

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> mesh;

	UPROPERTY(EditAnywhere)
	float orbSpeed;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<class ADiamondProjectPlayerController> Sender;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<class ADiamondProjectPlayerController> Receiver;
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	float TargetX;
};
