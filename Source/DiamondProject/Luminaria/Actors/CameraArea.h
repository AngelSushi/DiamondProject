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

	UPROPERTY(VisibleAnywhere)
	FVector2D MaxPosition;

	UPROPERTY(VisibleAnywhere)
	FVector2D MinPosition;	

protected:
	virtual void BeginPlay() override;

};
