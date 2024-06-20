#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DiamondProjectPlayerController.h"
#include "../Interface/ButtonInteractable.h"
#include "../Interface/InputDrawable.h"
#include "../CharacterStateMachine/CharacterStateMachine.h"
#include "../CharacterStateMachine/CharacterStateJump.h"
#include "DiamondProjectCharacter.generated.h"

class UPlayerManager;
class ADiamondProjectPlayerController;

UENUM(BlueprintType)
enum EDeathCause {
	NONE,
	SPIKE,
	ABSORBER,
	OTHER
};

UCLASS(Blueprintable)
class ADiamondProjectCharacter : public ACharacter,public IButtonInteractable, public IInputDrawable {
	GENERATED_BODY()

public:
	ADiamondProjectCharacter();

	virtual void BeginPlay() override;
	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

	virtual void Landed(const FHitResult& Hit) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void AbsorberInputStarted(FKey Key);

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UNiagaraComponent> DeathRespawnParticle;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UNiagaraComponent> LandOnGroundParticle;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBoxComponent> CameraDetection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPointLightComponent* Light;

	UPROPERTY()
	bool bIsOnGround;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMapWidget> MapWidgetClass;

	UPROPERTY()
	TObjectPtr<class UMapWidget> MapWidget;


	UFUNCTION(BlueprintCallable)
	void Death(EDeathCause DeathCause);

	UFUNCTION(BlueprintCallable)
	void Respawn(EDeathCause DeathCause);

	UFUNCTION(BlueprintCallable)
	void UpdateCheckpoint(ACheckpoint* checkpoint);

	UFUNCTION()
	void OnPlayerUpdateCheckpoint(ADiamondProjectCharacter* Character, ACheckpoint* Checkpoint);

	UFUNCTION()
	FVector GetCheckpoint() { return _checkPoint; }

	UFUNCTION()
	AActor* GetGroundActor() { return GroundActor; }

	UFUNCTION(BlueprintCallable)
	ALuminariaCamera* GetMainCamera() { return MainCamera; }

	UFUNCTION(BlueprintCallable,BlueprintPure)
	bool IsGrounded() { return bIsOnGround; }

	UFUNCTION(BlueprintPure)
	FColor GetPlayerColor() { return PlayerColor; }

	/* Light Functions */

	UFUNCTION(BlueprintCallable,BlueprintPure)
	float GetLightEnergy() { return LightEnergy; }

	UFUNCTION(BlueprintCallable)
	void SetLightEnergy(float NewLightEnergy) { LightEnergy = NewLightEnergy; }

	UFUNCTION(BlueprintCallable,BlueprintPure)
	float GetMinEnergy() { return MinEnergy; }

	UFUNCTION(BlueprintCallable,BlueprintPure)
	float GetMaxEnergy() { return MaxEnergy; }

	UFUNCTION(BlueprintCallable,BlueprintPure)
	float GetSharedEnergy() { return SharedEnergy; }

	UFUNCTION(BlueprintCallable,BlueprintPure)
	float GetLastLightValueReceive() { return LastLightValueReceive; }

	UFUNCTION(BlueprintCallable)
	void SetLastLightValueReceive(float NewLastLightValue) { LastLightValueReceive = NewLastLightValue; }

	UFUNCTION(BlueprintCallable,BlueprintPure)
	float GetScaleTarget() { return ScaleTarget; }

	UFUNCTION(BlueprintCallable)
	void SetScaleTarget(float NewScaleTarget) { ScaleTarget = NewScaleTarget; }

	UFUNCTION(BlueprintCallable,BlueprintPure)
	float GetLastLightValue() { return LastLightValue; }

	UFUNCTION(BlueprintCallable)
	void SetLastLightValue(float NewLastLightValue) { LastLightValue = NewLastLightValue; }

	UFUNCTION(BlueprintCallable,BlueprintPure)
	bool CanPush() { return bCanPush; }

	UFUNCTION(BlueprintCallable)
	void SetCanPush(bool NewCanPush) { bCanPush = NewCanPush; }

	UFUNCTION(BlueprintCallable,BlueprintPure)
	bool IsButtonPushPressed() { return bButtonPushPressed; }

	UFUNCTION(BlueprintCallable)
	void SetButtonPushPressed(bool ButtonPushPressed) { bButtonPushPressed = ButtonPushPressed; }

	UFUNCTION(BlueprintCallable)
	bool CanGrow() { return bCanGrow; }

	UFUNCTION(BlueprintCallable)
	void SetCanGrow(bool CanGrow) { bCanGrow = CanGrow; }

	UFUNCTION(BlueprintPure)
	float GetJumpDurationIncrease() { return JumpDurationIncrease; }

	UFUNCTION(BlueprintPure)
	float GetSpeedIncrease() { return SpeedIncrease; }


	UFUNCTION(BlueprintPure)
	float GetJumpMinDuration() {
		return GetStateMachine()->StateJump->GetJumpMinDuration();
	}

	UFUNCTION(BlueprintPure)
	float GetJumpMaxDuration() { 
		return GetStateMachine()->StateJump->GetJumpMaxDuration(); 
	}

	UFUNCTION(BlueprintCallable)
	void SetJumpMinDuration(float NewJumpMinDuration) {
		NewJumpMinDuration = FMath::Clamp(NewJumpMinDuration, 0, 1000);
		GetStateMachine()->StateJump->SetJumpMinDuration(NewJumpMinDuration);
	}

	UFUNCTION(BlueprintCallable)
	void SetJumpMaxDuration(float NewJumpMaxDuration) {
		NewJumpMaxDuration = FMath::Clamp(NewJumpMaxDuration, 0, 1000);
		GetStateMachine()->StateJump->SetJumpMaxDuration(NewJumpMaxDuration);
	}

	UFUNCTION(BlueprintPure)
	int GetJumpFactor() { // Light Energy
		if (LightEnergy == 50000.F) {
			return 0;
		}
		else if (LightEnergy == 35000.F || LightEnergy == 65000.F) {
			return 1;
		}
		else if (LightEnergy == 20000.F || LightEnergy == 80000.F) {
			return 2;
		}
		else if (LightEnergy == 5000.F || LightEnergy == 95000.F) {
			return 3;
		}
		else {
			return -1;
		}
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LightEnergy = 50000.F;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ADiamondProjectPlayerController* GetLuminariaController() {
		if (GetController()) {
			return Cast<ADiamondProjectPlayerController>(GetController());
		}

		return nullptr;
	}

	UFUNCTION(BlueprintPure)
	UPlayerAsset* GetPlayerAsset() { return PlayerAsset; }

	UFUNCTION()
	UCharacterStateMachine* GetStateMachine() { return CharacterStateMachine; }
	
	/* IInputDrawable Functions */
	virtual void EnableInputListener();
	virtual void DisableInputListener();

	UFUNCTION()
	virtual void CompleteInput(UInputUI* Input);
private:


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UPlayerAsset> PlayerAsset;

	UPROPERTY()
	UPlayerManager* PlayerManager;
	
	UPROPERTY()
	FVector _checkPoint;

	UPROPERTY()
	TObjectPtr<class ALuminariaCamera> MainCamera;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class AActor> GroundActor;

	UPROPERTY()
	TObjectPtr<class ACameraArea> LastHitArea;

	UPROPERTY(EditAnywhere)
	FColor PlayerColor;

	/* Light Variables */

	UPROPERTY()
	float MinEnergy = 0.F;

	UPROPERTY()
	float MaxEnergy = 100000.F;

	UPROPERTY(EditAnywhere)
	float SharedEnergy = 5000.F;

	UPROPERTY()
	float LastLightValueReceive = 50.F;

	UPROPERTY(EditAnywhere)
	float ScaleTarget = 1.F;

	UPROPERTY()
	float LastLightValue = 50.F;

	UPROPERTY()
	float JumpDurationIncrease = 0.05f;

	UPROPERTY()
	float SpeedIncrease = 40.F;

	UPROPERTY()
	bool bCanGrow = false;

	/* Push/Pull Variables */
	UPROPERTY(VisibleAnywhere)
	bool bCanPush;

	UPROPERTY()
	bool bButtonPushPressed;

	UPROPERTY()
	TObjectPtr<class UCharacterStateMachine> CharacterStateMachine;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnLandOnGround(ADiamondProjectCharacter* Character);
};

