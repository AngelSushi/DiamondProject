#pragma once

#include "CoreMinimal.h"
#include "DiamondProject/Luminaria/CameraBehaviors/CameraBehavior.h"
#include "CameraDynamicBehavior.generated.h"

class UPlayerEventsDispatcher;

USTRUCT()
struct  DIAMONDPROJECT_API FExtendData {

	GENERATED_BODY()

public:
	FVector position;
	FVector direction;
	uint8 id;

	FExtendData() : position(FVector::ZeroVector), direction(FVector::ZeroVector), id(0) {}
	FExtendData(const FVector& Position, const FVector& Direction,const int Id): position(Position), direction(Direction),id(Id) {}
	
	bool operator==(const FExtendData& other) const {
		return position == other.position && direction == other.direction && id == other.id;
	}

};

UCLASS()
class DIAMONDPROJECT_API UCameraDynamicBehavior : public UCameraBehavior {
	GENERATED_BODY()

public:
	virtual void BeginBehavior(ALuminariaCamera* Owner) override;

	virtual void OnPlayerMove(ADiamondProjectCharacter* character, FVector direction, bool& isCanceled) override;
	
	virtual void TickBehavior(float DeltaTime) override;

protected:
	void CameraBlock() override;

private:
	UPROPERTY()
	float OffsetX;

	UPROPERTY()
	TArray<FExtendData> _extendPositions;

	UFUNCTION()
	void CalculateOffsideFrustumOffset(ADiamondProjectCharacter* character, FVector direction);

	UFUNCTION()
	float Approach(float Current, float Target, float Incr);
};
