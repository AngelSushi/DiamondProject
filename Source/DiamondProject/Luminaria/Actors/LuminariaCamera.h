#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "LuminariaCamera.generated.h"

UENUM(BlueprintType)
enum ECameraBehavior : uint8
{
	DEFAULT,
	LEADER,
	DYNAMIC,
	GOTO,
};

UCLASS()
class DIAMONDPROJECT_API ALuminariaCamera : public ACameraActor {
	GENERATED_BODY()

public:

	ALuminariaCamera();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class ACameraArea> CurrentArea;

	UPROPERTY(EditAnywhere)
	bool bDebugCamera;

	UFUNCTION()
	void OnPlayerDeath(ADiamondProjectCharacter* Character);

	UFUNCTION()
	void OnPlayerRegister(ADiamondProjectCharacter* Character);

	/* State Machine Variable */
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TEnumAsByte<ECameraBehavior> BehaviorState;

	UPROPERTY()
	TObjectPtr<class UCameraBehavior> CameraBehavior;

	UPROPERTY()
	TObjectPtr<class UCameraDynamicBehavior> DynamicBehavior;

	UPROPERTY()
	TObjectPtr<class UCameraDefaultBehavior> DefaultBehavior;

	UPROPERTY()
	TObjectPtr<class UGoToBehavior> GoToBehavior;

	UPROPERTY()
	TObjectPtr<class UCameraLeaderBehavior> LeaderBehavior;

	UPROPERTY()
	TObjectPtr<class UHeightCameraBehavior> HeightBehavior;

	/* State Machine Functions */

	void SwitchBehavior(TSubclassOf<class UCameraBehavior> Component,TFunction<void(UCameraBehavior* AddedComponent)> ResultFunc = [](UCameraBehavior* CameraBehavior) {});

	void InitBehavior();

private:
	UPROPERTY()
	TArray<ADiamondProjectCharacter*> Characters;

	UPROPERTY()
	FVector StartPosition;

	UPROPERTY()
	bool bHasDead;
};
