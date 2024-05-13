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

	UPROPERTY(EditAnywhere)
	float PlayerSpeed = 600.F;

	UPROPERTY(VisibleAnywhere)
	FVector2D MinPosition;

	UPROPERTY(VisibleAnywhere)
	FVector2D MaxPosition;

	UFUNCTION()
	bool HasVisited() { return bHasVisited; }

	UFUNCTION(BlueprintCallable)
	bool CanGrow() { return bCanGrow; }

	UFUNCTION(BlueprintCallable)
	void TickArea(float DeltaTime);

	UFUNCTION()
	void SetVisited(bool HasVisited) { bHasVisited = HasVisited; }

	/* Draw Functions */

	UFUNCTION(BlueprintCallable,BlueprintPure)
	FVector2D GetTopLeftCorner() { return TopLeftCorner; }

	UFUNCTION()
	void SetTopLeftCorner(FVector2D NewTopLeftCorner) { TopLeftCorner = NewTopLeftCorner; }

	UFUNCTION(BlueprintCallable,BlueprintPure)
	FVector2D GetTopRightCorner() { return TopRightCorner; }

	UFUNCTION()
	void SetTopRightCorner(FVector2D NewTopRightCorner) { TopRightCorner = NewTopRightCorner; }

	UFUNCTION(BlueprintCallable,BlueprintPure)
	FVector2D GetBotLeftCorner() { return BotLeftCorner; }

	UFUNCTION()
	void SetBotLeftCorner(FVector2D NewBotLeftCorner) { BotLeftCorner = NewBotLeftCorner; }

	UFUNCTION(BlueprintCallable,BlueprintPure)
	FVector2D GetBotRightCorner() { return BotRightCorner; }

	UFUNCTION()
	void SetBotRightCorner(FVector2D NewBotRightCorner) { BotRightCorner = NewBotRightCorner; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	bool bHasVisited;

	UPROPERTY(EditAnywhere)
	bool bCanGrow;

	UPROPERTY()
	UPlayerManager* PlayerManager;

	/* Draw Variables */

	UPROPERTY()
	FVector2D TopLeftCorner;

	UPROPERTY()
	FVector2D TopRightCorner;

	UPROPERTY()
	FVector2D BotLeftCorner;

	UPROPERTY()
	FVector2D BotRightCorner;
};
