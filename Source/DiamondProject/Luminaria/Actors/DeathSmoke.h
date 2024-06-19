#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeathSmoke.generated.h"

UCLASS()
class DIAMONDPROJECT_API ADeathSmoke : public AActor {
	GENERATED_BODY()
	
public:	
	ADeathSmoke();

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> Root;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UNiagaraComponent> NiagaraSystem;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UDeathComponent> DeathComponent;

protected:
	virtual void BeginPlay() override;

public:	

};
