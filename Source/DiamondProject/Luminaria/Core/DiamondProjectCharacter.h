#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DiamondProjectCharacter.generated.h"

class UPlayerManager;

UCLASS(Blueprintable)
class ADiamondProjectCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ADiamondProjectCharacter();

	virtual void BeginPlay() override;
	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UFUNCTION(BlueprintCallable)
	void Death();

	UFUNCTION(BlueprintCallable)
	void UpdateCheckpoint(ACheckpoint* checkpoint);

	UFUNCTION()
	void OnPlayerUpdateCheckpoint(ADiamondProjectCharacter* Character, ACheckpoint* Checkpoint);

	UFUNCTION()
	FVector GetCheckpoint() { return _checkPoint; }

	UFUNCTION()
	AActor* GetGroundActor() { return GroundActor; }

	UPROPERTY()
	bool bIsOnGround;

	UPROPERTY()
	bool bIsOnGroundLastTick;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> MapWidgetClass;

	UPROPERTY()
	TObjectPtr<class UUserWidget> MapWidget;


private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY()
	UPlayerManager* PlayerManager;
	
	UPROPERTY()
	FVector _checkPoint;

	UPROPERTY()
	TObjectPtr<class ALuminariaCamera> MainCamera;

	UPROPERTY()
	TObjectPtr<class AActor> GroundActor;

	UPROPERTY()
	TObjectPtr<class ACameraArea> LastHitArea;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};

