#pragma once

#include "CoreMinimal.h"
#include "DiamondProject/Luminaria/ActorComponents/CameraBehavior.h"
#include "CameraDynamicBehavior.generated.h"


USTRUCT()
struct  DIAMONDPROJECT_API FExtendData {

	GENERATED_BODY()

public:
	FVector position;
	FVector direction;
	FVector cameraPosition;

	FExtendData() : position(FVector::ZeroVector), direction(FVector::ZeroVector), cameraPosition(FVector::ZeroVector) {}
	FExtendData(const FVector& Position, const FVector& Direction, const FVector& CameraPosition): position(Position), direction(Direction),cameraPosition(CameraPosition) {}
	
	bool operator==(const FExtendData& other) const {
		return position == other.position && direction == other.direction && cameraPosition == other.cameraPosition;
	}

};

UCLASS()
class DIAMONDPROJECT_API UCameraDynamicBehavior : public UCameraBehavior {
	GENERATED_BODY()

	UCameraDynamicBehavior();

public:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRegisterPlayer(ADiamondProjectCharacter* player);

	UFUNCTION()
	void OnPlayerMove(ADiamondProjectCharacter* character, FVector direction, bool& isCanceled);
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	FVector _offset;

	UPROPERTY()
	bool _canExtend;

	UPROPERTY()
	TArray<FExtendData> _extendPositions;

	UFUNCTION()
	void CalculateOffsideFrustumOffset(ADiamondProjectCharacter* character, FVector direction);
};
