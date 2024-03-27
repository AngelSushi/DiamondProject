#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeathAreaActor.generated.h"

UCLASS()
class DIAMONDPROJECT_API ADeathAreaActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADeathAreaActor();

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> Root;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UDeathComponent> DeathComponent;


protected:
	virtual void BeginPlay() override;

};
