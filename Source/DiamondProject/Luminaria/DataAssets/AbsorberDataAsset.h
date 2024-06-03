// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AbsorberDataAsset.generated.h"

enum EInput;

UCLASS()
class DIAMONDPROJECT_API UAbsorberDataAsset : public UDataAsset {
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, meta = (ForceUnits = "cm"))
	float RadiusDetection = 750.F;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "1"))
	float AbsorberForce = 0.8F;

	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EInput>> PossibleInputs;

	UPROPERTY(EditAnywhere)
	float MaxStunTimer = 5.F;
	
};
