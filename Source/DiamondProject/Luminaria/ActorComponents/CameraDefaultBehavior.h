#pragma once

#include "CoreMinimal.h"
#include "DiamondProject/Luminaria/ActorComponents/CameraBehavior.h"
#include "CameraDefaultBehavior.generated.h"

UCLASS()
class DIAMONDPROJECT_API UCameraDefaultBehavior : public UCameraBehavior
{
	GENERATED_BODY()

	UCameraDefaultBehavior();
	
public:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void RegisterPlayer(ADiamondProjectCharacter* character);

	UFUNCTION()
	void OnPlayerMove(ADiamondProjectCharacter* character,FVector direction,bool& isCanceled);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
