#pragma once

#include "CoreMinimal.h"
#include "DiamondProject/Luminaria/ActorComponents/CameraBehavior.h"
#include "CameraDynamicBehavior.generated.h"


UCLASS()
class DIAMONDPROJECT_API UCameraDynamicBehavior : public UCameraBehavior
{
	GENERATED_BODY()

	UCameraDynamicBehavior();

public:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRegisterPlayer(ACharacter* player);
	
	UFUNCTION()
	void OnPlayerMove(ACharacter* player, FVector2D direction,bool& isCanceled);
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	TArray<ACharacter*> _characters;

	UPROPERTY()
	FVector _barycenter;

	UPROPERTY()
	float _minZoomDistance;

	UPROPERTY()
	float _maxZoomDistance;

	UPROPERTY()
	FVector _defaultCameraPosition;

	UFUNCTION()
	void CalculateZoomDistance(float& minDistance, float& maxDistance);
};
