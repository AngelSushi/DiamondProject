#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElectricityOrb.generated.h"

UCLASS()
class DIAMONDPROJECT_API AElectricityOrb : public AActor {
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> _mesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USphereComponent> _sphereCollision;

	UPROPERTY()
	bool _hasBeenSet;

public:	
	AElectricityOrb();

	UPROPERTY(EditAnywhere)
	float orbSpeed;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<class ADiamondProjectPlayerController> orbSender;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<class ADiamondProjectPlayerController> orbReceiver;
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
