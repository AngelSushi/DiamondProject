#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DiamondProjectPlayerController.h"
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
class ADiamondProjectCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ADiamondProjectCharacter();

	virtual void BeginPlay() override;
	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

	virtual void Landed(const FHitResult& Hit) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

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
	float GetGravityScaleSaved() { return GravityScaleSaved; }

	UFUNCTION(BlueprintCallable)
	void SetGravityScaleSaved(float NewGravityScaleSaved) { GravityScaleSaved = NewGravityScaleSaved; }

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LightEnergy = 50000.F;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;


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

	/* Light Variables */

	UPROPERTY(EditAnywhere)
	float MinEnergy = 0.F;

	UPROPERTY(EditAnywhere)
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
	float GravityScaleSaved = 1.5F;

	UPROPERTY()
	bool bCanGrow = false;

	/* Push/Pull Variables */
	UPROPERTY(VisibleAnywhere)
	bool bCanPush;

	UPROPERTY()
	bool bButtonPushPressed;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnLandOnGround(ADiamondProjectCharacter* Character);

	UFUNCTION(BlueprintCallable,BlueprintPure)
	ADiamondProjectPlayerController* GetLuminariaController() { 
		if (GetController()) {
			return Cast<ADiamondProjectPlayerController>(GetController());
		}

		return nullptr;
	}
};

