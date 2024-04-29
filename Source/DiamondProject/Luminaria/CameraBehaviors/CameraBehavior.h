#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraBehavior.generated.h"


class ALuminariaCamera;
class ADiamondProjectCharacter;

UCLASS( ClassGroup=(Custom),Blueprintable, meta=(BlueprintSpawnableComponent) )
class DIAMONDPROJECT_API UCameraBehavior : public UObject {
	GENERATED_BODY()

public:	

	 virtual void BeginBehavior(ALuminariaCamera* Owner);
	 virtual void TickBehavior(float DeltaTime);

protected:
	UPROPERTY()
	TObjectPtr<ALuminariaCamera> OwnerActor;

	UPROPERTY()
	FVector ForwardDirection;

	UFUNCTION()
	void CalculateBarycenter();

	UPROPERTY()
	TArray<ADiamondProjectCharacter*> _characters;

	UPROPERTY()
	FVector Barycenter;
public:	

	UFUNCTION()
	virtual void OnPlayerMove(ADiamondProjectCharacter* character, FVector direction, bool& isCanceled);

	UFUNCTION()
	void SetBarycenter(FVector NewBarycenter) { Barycenter = NewBarycenter; }

protected:
	UPROPERTY()
	float DefaultX;

	UPROPERTY()
	float DefaultZ;

	UPROPERTY()
	TObjectPtr<class UPlayerManager> PlayerManager;

	UPROPERTY()
	bool bBlock;

	UFUNCTION()
	float Approach(float Current, float Target, float Incr);

private:
	UFUNCTION()
	bool IsInFrustum(ADiamondProjectCharacter* Character,FVector Position);
};
