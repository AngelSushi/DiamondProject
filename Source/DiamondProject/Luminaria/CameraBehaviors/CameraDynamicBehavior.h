#pragma once

#include "CoreMinimal.h"
#include "DiamondProject/Luminaria/CameraBehaviors/CameraBehavior.h"
#include "CameraDynamicBehavior.generated.h"

class UPlayerEventsDispatcher;

USTRUCT()
struct  DIAMONDPROJECT_API FExtendData {

	GENERATED_BODY()

public:
	FVector Position;
	FVector Direction;
	uint8 Id;
	float Offset;

	FExtendData() : Position(FVector::ZeroVector), Direction(FVector::ZeroVector), Id(0), Offset(0) {}
	FExtendData(const FVector& Position, const FVector& Direction,const int Id,const float Offset): Position(Position), Direction(Direction),Id(Id),Offset(Offset) {}
	
	bool operator==(const FExtendData& other) const {
		return Position == other.Position && Direction == other.Direction && Id == other.Id && Offset == other.Offset;
	}

};

UCLASS()
class DIAMONDPROJECT_API UCameraDynamicBehavior : public UCameraBehavior {
	GENERATED_BODY()

public:
	virtual void BeginBehavior(ALuminariaCamera* Owner) override;

	void OnPlayerMove(ADiamondProjectCharacter* character, FVector2D Input, FVector direction, bool& isCanceled) override;

	UFUNCTION()
	void OnPlayerDeath(ADiamondProjectCharacter* Character,EDeathCause DeathCause);

	UFUNCTION()
	void OnPlayerRespawn(ADiamondProjectCharacter* Character, EDeathCause DeathCause, FVector RespawnPosition);
	
	virtual void TickBehavior(float DeltaTime) override;

private:
	UPROPERTY()
	float OffsetX;

	UPROPERTY()
	bool bIsDead;

	UPROPERTY()
	TArray<FExtendData> _extendPositions;

	UFUNCTION()
	void CalculateOffsideFrustumOffset(ADiamondProjectCharacter* character, FVector direction);
};
