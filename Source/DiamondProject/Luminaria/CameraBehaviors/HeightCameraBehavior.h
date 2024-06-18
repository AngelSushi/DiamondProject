#pragma once

#include "CoreMinimal.h"
#include "DiamondProject/Luminaria/CameraBehaviors/CameraBehavior.h"
#include "HeightCameraBehavior.generated.h"

UCLASS()
class DIAMONDPROJECT_API UHeightCameraBehavior : public UCameraBehavior {
	GENERATED_BODY()
	
public:
	virtual void BeginBehavior(ALuminariaCamera* Owner) override;

	virtual void TickBehavior(float DeltaTime) override;

	UFUNCTION()
	int GetOffsetZ() { return OffsetZ; }

private:

	UPROPERTY()
	FVector LinePositionTop;

	UPROPERTY()
	FVector LinePositionBot;

	UFUNCTION()
	void DetermineLimit();

	UFUNCTION()
	void OnMovePlayer(ADiamondProjectCharacter* Character);

	UPROPERTY()
	TArray<ADiamondProjectCharacter*> ExceedCharacters;

	UPROPERTY()
	float OffsetZ;	

	UPROPERTY()
	bool bChangeLimit;

	UFUNCTION()
	float CalculateOffset(ADiamondProjectCharacter* Character,ADiamondProjectCharacter* Other,FVector Start);

	bool bTestLimit;

	void SetOffsetZ(float NewOffsetZ) {
		if (NewOffsetZ == 0) {
			GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Red, TEXT("[HeightCamera] Offset that we try to set is null"));
			return;
		}

		OffsetZ = NewOffsetZ;
	}
};
