#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Absorber.generated.h"

UCLASS()
class DIAMONDPROJECT_API AAbsorber : public AActor
{
	GENERATED_BODY()
	
public:	
	AAbsorber();

	UFUNCTION(BlueprintPure)
	float GetAbsorberForce() { return AbsorberForce; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, meta = (ForceUnits = "cm"))
	float RadiusDetection = 750.F;

	UPROPERTY(EditAnywhere,meta = (ClampMin = "0", ClampMax = "1"))
	float AbsorberForce = 0.8F;

	UPROPERTY()
	TObjectPtr<class UPlayerManager> PlayerManager;

	UPROPERTY()
	TObjectPtr<class ADiamondProjectCharacter> DetectedPlayer;

	UPROPERTY()
	TObjectPtr<class UAbsorberEventsDispatcher> AbsorberEventsDispatcher;
};
