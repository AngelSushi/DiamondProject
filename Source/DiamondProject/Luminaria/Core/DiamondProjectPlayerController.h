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

	UFUNCTION(BlueprintCallable,BlueprintPure)
	bool IsJumpPressed() { return bIsJumpPressed; }

	UFUNCTION(BlueprintCallable,BlueprintPure)
	float GetFallGravityScale() { return FallGravityScale; }

	UFUNCTION(BlueprintCallable,BlueprintPure)
	bool IsFalling() { return bIsFalling; }

	UFUNCTION(BlueprintCallable)
	void SetIsFalling(bool IsFalling) { bIsFalling = IsFalling; }

	UFUNCTION(BlueprintCallable,BlueprintPure)
	float GetJumpOffsetPressed() { return JumpOffsetPressed; }

	UFUNCTION(BlueprintCallable,BlueprintPure)
	bool HasAugmentedHeight() { return bHasAugmentedHeight; }

	UFUNCTION(BlueprintCallable)
	void SetHasAugmentedHeight(bool HasAugmentedHeight) { bHasAugmentedHeight = HasAugmentedHeight; }

	UFUNCTION(BlueprintCallable,BlueprintPure)
	FVector2D GetMoveValue() { return MoveValue; }

	UFUNCTION(BlueprintCallable)
	void SetIsPushing(bool IsPushing) { bIsPushing = IsPushing; }

	UFUNCTION(BlueprintCallable)
	void SetIsPulling(bool IsPulling) { bIsPulling = IsPulling; }


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

	/*UFUNCTION()
	void Push();

	UFUNCTION()
	void StopPush();

	UFUNCTION()
	void Pull();

	UFUNCTION()
	void StopPull();
	*/

	UPROPERTY(EditAnywhere)
	bool isUsingDepthMovement;

	UPROPERTY()
	bool bIsMapOpen;

	UPROPERTY()
	UPlayerManager* PlayerManager;

	UPROPERTY()
	FVector2D MoveValue;

	/* Jump Variables */
	UPROPERTY()
	bool bIsJumpPressed;

	UPROPERTY(EditAnywhere)
	float FallGravityScale = 1.0F;

	UPROPERTY(EditAnywhere)
	float JumpOffsetPressed = 0.5F;

	UPROPERTY()
	bool bHasAugmentedHeight;

	UPROPERTY()
	bool bIsFalling;
};


