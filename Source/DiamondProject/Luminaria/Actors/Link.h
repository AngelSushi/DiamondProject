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
	
private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> _mesh;

	UPROPERTY()
	TArray<ADiamondProjectCharacter*> _characters;

	UFUNCTION()
	void RegisterPlayer(ADiamondProjectCharacter* character);

	UFUNCTION()
	void CalculateBarycenter();

	UFUNCTION()
	void OnPlayerMove(ADiamondProjectCharacter* Character, FVector Direction, bool& IsCanceled);

	UPROPERTY()
	FVector _barycenter;

	UPROPERTY(EditAnywhere)
	float DistanceMax;

	UPROPERTY()
	UPlayerManager* PlayerManager;

public:	
	ALink();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
