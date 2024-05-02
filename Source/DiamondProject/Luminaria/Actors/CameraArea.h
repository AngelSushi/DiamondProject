#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../../DiamondProject/Luminaria/Actors/LuminariaCamera.h"
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
	TObjectPtr<class USceneComponent> GoTo;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<class UBoxComponent> BoxCollision;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECameraBehavior> AreaBehavior;

	UPROPERTY(EditAnywhere)
	uint8 PlayerNeeded;

	UPROPERTY(EditAnywhere)
	uint16 ZoomMin;

	UPROPERTY(EditAnywhere)
	uint16 ZoomMax;

	UPROPERTY(VisibleAnywhere)
	FVector2D MinPosition;

	UPROPERTY(VisibleAnywhere)
	FVector2D MaxPosition;

	UFUNCTION()
	bool HasVisited() { return bHasVisited; }

	UFUNCTION()
	void SetVisited(bool HasVisited) { bHasVisited = HasVisited; }
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	bool bHasVisited;
};
