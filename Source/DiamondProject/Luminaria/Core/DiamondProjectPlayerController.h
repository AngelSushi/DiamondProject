// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "DiamondProjectPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class UInputMappingContext;
class UPlayerManager;
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* OpenMapAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PushAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PullAction;


	UPROPERTY()
	FVector LastDirection;

	UPROPERTY()
	FVector MovementDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanFlip = true;

	bool bIsLookingLeft;

	UFUNCTION()
	bool IsJumping() { return bIsJumping; }

	UFUNCTION()
	void SetJumping(bool Jumping) { bIsJumping = Jumping; }

	UPROPERTY() // Mettre privé
	bool bIsJumping;

protected:

	virtual void SetupInputComponent() override;
	
	virtual void BeginPlay();
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsPushing;

	UPROPERTY(BlueprintReadWrite)
	bool bIsPulling;

private:

	UFUNCTION(BlueprintCallable,BlueprintPure)
	ADiamondProjectCharacter* GetPlayer() { return Cast<ADiamondProjectCharacter>(GetCharacter()); }


	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Jump();

	UFUNCTION()
	void StopJump();

	UFUNCTION()
	void OpenMap();

	UFUNCTION()
	void Push();

	UFUNCTION()
	void StopPush();

	UFUNCTION()
	void Pull();

	UFUNCTION()
	void StopPull();

	UPROPERTY(EditAnywhere)
	bool isUsingDepthMovement;

	UPROPERTY()
	bool bIsMapOpen;

	UPROPERTY()
	UPlayerManager* PlayerManager;

};


