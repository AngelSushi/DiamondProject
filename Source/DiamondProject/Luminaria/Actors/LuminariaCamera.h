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
class DIAMONDPROJECT_API ALuminariaCamera : public ACameraActor
{
	GENERATED_BODY()

public:

	ALuminariaCamera();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TEnumAsByte<ECameraBehavior> CameraBehavior;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<ECameraBehavior> LastBehavior;

	UPROPERTY(BlueprintReadWrite)
	UActorComponent* LastBehaviorComponent;

	UFUNCTION()
	void OnPlayerDeath(ADiamondProjectCharacter* Character);
	
	UFUNCTION()
	void OnPlayerRegister(ADiamondProjectCharacter* Character);

	void AddComponent(TSubclassOf<class UCameraBehavior> Component,TFunction<void(UActorComponent* AddedComponent)> ResultFunc);

private:
	UPROPERTY()
	TArray<ADiamondProjectCharacter*> Characters;

	UPROPERTY()
	FVector StartPosition;

	UPROPERTY()
	bool bHasDead;
};
