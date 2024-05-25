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

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<class USceneComponent> GoTo;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<class UBoxComponent> BoxCollision;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<class USceneComponent> SpawnPoint;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	int64 Id;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TEnumAsByte<ECameraBehavior> AreaBehavior;

	UPROPERTY(EditAnywhere)
	uint8 PlayerNeeded;

	UPROPERTY(EditAnywhere)
	uint16 ZoomMin;

	UFUNCTION(BlueprintPure)
	int GetZoomMin() { return ZoomMin; }

	UPROPERTY(EditAnywhere)
	uint16 ZoomMax;

	UPROPERTY(EditAnywhere)
	float PlayerSpeedMin = 600.F;

	UPROPERTY(EditAnywhere)
	float PlayerSpeedMax = 600.F;

	UPROPERTY(EditAnywhere)
	float TransitionDuration = 50.F;

	UPROPERTY(EditAnywhere)
	float ZoomDuration = 8.F;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	FVector2D MinPosition;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	FVector2D MaxPosition;

	UFUNCTION()
	bool HasVisited() { return bHasVisited; }

	//UFUNCTION(BlueprintCallable)
	//bool CanGrow() { return bCanGrow; }

	UFUNCTION(BlueprintCallable)
	void TickArea(float DeltaTime);

	UFUNCTION()
	void SetVisited(bool HasVisited) { bHasVisited = HasVisited; }
protected:
	virtual void BeginPlay() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	UPROPERTY(VisibleAnywhere)
	bool bHasVisited;

	//UPROPERTY(EditAnywhere)
	//bool bCanGrow;

	UPROPERTY()
	UPlayerManager* PlayerManager;
};
