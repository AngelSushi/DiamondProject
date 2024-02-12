// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DiamondProject/Luminaria/ActorComponents/CameraBehavior.h"
#include "CameraDefaultBehavior.generated.h"

/**
 * 
 */
UCLASS()
class DIAMONDPROJECT_API UCameraDefaultBehavior : public UCameraBehavior
{
	GENERATED_BODY()

	UCameraDefaultBehavior();
	
public:
	virtual void BeginPlay() override;
	
	UPROPERTY()
	TArray<ACharacter*> _characters;
	
	UFUNCTION()
	void RegisterPlayer(ACharacter* character);

	UFUNCTION()
	void OnPlayerMove(ACharacter* character,FVector2D direction,bool& isCanceled);

private:
	UPROPERTY()
	float _defaultY;
};
