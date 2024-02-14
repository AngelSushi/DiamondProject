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
	void OnRegisterPlayer(ADiamondProjectCharacter* player);
	
	UFUNCTION()
	void OnPlayerMove(ADiamondProjectCharacter* player, FVector2D direction,bool& isCanceled);
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	TArray<ADiamondProjectCharacter*> _characters;

	UPROPERTY()
	FVector _barycenter;

	UPROPERTY(EditAnywhere)
	float _minZoomDistance;

	UPROPERTY()
	float _maxZoomDistance;

	UPROPERTY()
	FVector _defaultCameraPosition;
};
