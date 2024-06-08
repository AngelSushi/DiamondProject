#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Link.generated.h"

class UPlayerManager;
UCLASS()
class DIAMONDPROJECT_API ALink : public AActor{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> Root;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UStaticMeshComponent> _mesh;

	UFUNCTION()
	float GetDistanceMax() const { return DistanceMax; }

	UFUNCTION(BlueprintPure)
	float GetDistanceAlpha() { return DistanceAlpha; }
private:

	UPROPERTY()
	TArray<ADiamondProjectCharacter*> _characters;

	UFUNCTION()
	void RegisterPlayer(ADiamondProjectCharacter* character);

	UFUNCTION()
	void CalculateBarycenter();

	UFUNCTION()
	void OnPlayerMove(ADiamondProjectCharacter* Character,FVector2D Input, FVector Direction, bool& IsCanceled);

	UPROPERTY()
	FVector _barycenter;

	UPROPERTY(EditAnywhere)
	float DistanceMax;

	UPROPERTY()
	float CrushMin = 0.1F;

	UPROPERTY()
	float CrushMax = 0.05F;

	UPROPERTY()
	float DistanceAlpha;

	UPROPERTY()
	TMap<ADiamondProjectCharacter*, FVector> PlayersLastDirection;

	UPROPERTY()
	UPlayerManager* PlayerManager;

public:	
	ALink();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
