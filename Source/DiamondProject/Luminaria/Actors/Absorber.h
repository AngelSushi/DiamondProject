#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Absorber.generated.h"

enum EInput;
enum EDeathCause;

UCLASS()
class DIAMONDPROJECT_API AAbsorber : public AActor {
	GENERATED_BODY()
	
public:	
	AAbsorber();

	UFUNCTION(BlueprintPure)
	float GetAbsorberForce() { return AbsorberForce; }

	UFUNCTION()
	EInput GetCurrentInput() { return CurrentInput; }

	UFUNCTION()
	UAbsorberDataAsset* GetAbsorberData() { return AbsorberAsset; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void GenerateInput();

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UDeathComponent> DeathComponent;

	UFUNCTION()
	void OnStunAbsorber(AAbsorber* Absorber);

	UFUNCTION()
	void OnDeStunAbsorber(AAbsorber* Absorber);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerDeathCancellable(ADiamondProjectCharacter* Character, EDeathCause DeathCause, bool& IsCanceled);

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> Root;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USkeletalMeshComponent> Mesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAbsorberDataAsset> AbsorberAsset;

	UPROPERTY()
	float RadiusDetection;

	UPROPERTY()
	float AbsorberForce;

	UPROPERTY()
	TArray<TEnumAsByte<EInput>> PossibleInputs;

	UPROPERTY()
	float MaxStunTimer;


	UPROPERTY()
	TObjectPtr<class UPlayerManager> PlayerManager;

	UPROPERTY()
	TObjectPtr<class ADiamondProjectCharacter> DetectedPlayer;

	UPROPERTY()
	TObjectPtr<class UAbsorberEventsDispatcher> AbsorberEventsDispatcher;

	UPROPERTY(VisibleAnywhere)
	TEnumAsByte<EInput> CurrentInput;

	UPROPERTY()
	TEnumAsByte<EInput> LastInput;

	UPROPERTY()
	bool bIsStun;

	UPROPERTY()
	float StunTimer;
};
