#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spike.generated.h"

UCLASS()
class DIAMONDPROJECT_API ASpike : public AActor {
	GENERATED_BODY()
	
public:	
	ASpike();

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UDeathComponent> DeathComponent;

protected:
	virtual void BeginPlay() override;


};
