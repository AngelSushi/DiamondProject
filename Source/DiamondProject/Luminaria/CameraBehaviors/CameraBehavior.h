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
	FVector _barycenter;

public:	

	UFUNCTION()
	virtual void OnPlayerMove(ADiamondProjectCharacter* character, FVector direction, bool& isCanceled);

protected:
	UPROPERTY()
	float _defaultY;

	UPROPERTY()
	TObjectPtr<class UPlayerEventsDispatcher> PlayerEventsDispatcher;

	UPROPERTY()
	bool bBlock;

	UFUNCTION()
	virtual void CameraBlock();

private:
	UFUNCTION()
	bool IsInFrustum(ADiamondProjectCharacter* Character,FVector Position);
};
