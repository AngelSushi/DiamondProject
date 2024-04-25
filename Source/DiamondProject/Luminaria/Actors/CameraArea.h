#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraArea.generated.h"

class UBoxComponent;

UCLASS()
class DIAMONDPROJECT_API ACameraArea : public AActor {
	GENERATED_BODY()
	
public:	
	ACameraArea();

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> Root;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBoxComponent> BoxCollision;

	UPROPERTY(EditAnywhere)
	uint16 ZoomMin;

	UPROPERTY(EditAnywhere)
	uint16 ZoomMax;

	UPROPERTY(VisibleAnywhere)
	FVector2D MinPosition;

	UPROPERTY(VisibleAnywhere)
	FVector2D MaxPosition;


protected:
	virtual void BeginPlay() override;

};
