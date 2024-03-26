#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DiamondProjectCharacter.generated.h"

class UPlayerEventsDispatcher;

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


private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY()
	UPlayerEventsDispatcher* PlayerEventsDispatcher;
	
	UPROPERTY()
	FVector _checkPoint;
};

