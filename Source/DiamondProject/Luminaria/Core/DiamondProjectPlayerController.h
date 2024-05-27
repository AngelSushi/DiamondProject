#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "DiamondProjectPlayerController.generated.h"

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


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MovementActionGamepad;

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

	UFUNCTION(BlueprintPure)
	float GetJumpMinDuration() { return JumpMinDuration; }

	UFUNCTION(BlueprintPure)
	float GetJumpMaxDuration() { return JumpMaxDuration; }

	UFUNCTION(BlueprintCallable)
	void SetJumpMinDuration(float NewJumpMinDuration) { JumpMinDuration = NewJumpMinDuration; }

	UFUNCTION(BlueprintCallable)
	void SetJumpMaxDuration(float NewJumpMaxDuration) { JumpMaxDuration = NewJumpMaxDuration; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ADiamondProjectCharacter* GetPlayer() { return Cast<ADiamondProjectCharacter>(GetCharacter()); }

protected:

	virtual void SetupInputComponent() override;
	
	virtual void BeginPlay();
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsPushing;

	UPROPERTY(BlueprintReadWrite)
	bool bIsPulling;

private:

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Jump();

	UFUNCTION()
	void StopJump();

	UFUNCTION()
	void OnInputJumpReleased();

	UFUNCTION()
	void OpenMap();

	UPROPERTY()
	bool bIsMapOpen;

	UPROPERTY()
	UPlayerManager* PlayerManager;

	UPROPERTY()
	FVector2D MoveValue;

	/* Jump Variables */
	UPROPERTY()
	bool bIsJumpPressed;

	UPROPERTY()
	float FallGravityScale = 1.0F;

	UPROPERTY()
	float JumpOffsetPressed = 0.5F;

	UPROPERTY()
	bool bHasAugmentedHeight;

	UPROPERTY()
	bool bIsFalling;

	UPROPERTY()
	float JumpMinDuration;

	UPROPERTY()
	float JumpMaxDuration;

	UPROPERTY()
	float JumpTimer;
};


