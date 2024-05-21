#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevitationActor.generated.h"

UCLASS()
class DIAMONDPROJECT_API ALevitationActor : public AActor {
	GENERATED_BODY()
	
public:	
	ALevitationActor();

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> MeshComponent;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	FVector OriginPosition;

	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
	FVector LevitationPosition;

	UPROPERTY(EditAnywhere)
	float LevitationSpeed = 30.F;

	UPROPERTY()
	float OriginSpeed;

	UPROPERTY()
	float LevitationTimer;

};
