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

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere)
	float RadiusDetection;

	UPROPERTY()
	TObjectPtr<class UPlayerManager> PlayerManager;
};
