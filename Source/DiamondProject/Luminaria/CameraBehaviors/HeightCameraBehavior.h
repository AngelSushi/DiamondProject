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
	void OnPlayerLandOnGround(ADiamondProjectCharacter* Character);

	UPROPERTY()
	TArray<ADiamondProjectCharacter*> ExceedCharacters;

	UPROPERTY()
	float OffsetZ;	

	UPROPERTY()
	bool bChangeLimit;

	UFUNCTION()
	float CalculateOffset(ADiamondProjectCharacter* Character,ADiamondProjectCharacter* Other,FVector Start);
};
