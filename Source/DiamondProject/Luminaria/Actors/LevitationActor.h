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
	float LevitationSpeed;

	UPROPERTY()
	float CalculateSpeed;

	UPROPERTY()
	bool bDescent;

	UFUNCTION()
	FVector PingPong(FVector& Start, FVector& End, float Speed);

};
