// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "DiamondProjectPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class UInputMappingContext;
class UPlayerEventsDispatcher;
class UInputAction;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS()
class ADiamondProjectPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ADiamondProjectPlayerController();
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* PlayerMappingContext;
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MovementAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

protected:

	virtual void SetupInputComponent() override;
	
	virtual void BeginPlay();

private:

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Jump();

	UFUNCTION()
	void StopJump();

	UPROPERTY(EditAnywhere)
	bool isUsingDepthMovement;

	UPROPERTY()
	UPlayerEventsDispatcher* PlayerEventsDispatcher;

};


