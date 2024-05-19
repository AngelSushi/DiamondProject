#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DialogSpawner.generated.h"

class UBillboardComponent;
class UPlayerManager;

UCLASS()
class DIAMONDPROJECT_API ADialogSpawner : public AActor {
	GENERATED_BODY()
	
public:	
	ADialogSpawner();

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> Root;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBillboardComponent> Billboard;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UArrowComponent> Arrow;


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY()
	UPlayerManager* PlayerManager;
};
