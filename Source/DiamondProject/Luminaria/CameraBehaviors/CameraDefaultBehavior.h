#pragma once

#include "CoreMinimal.h"
#include "DiamondProject/Luminaria/CameraBehaviors/CameraBehavior.h"
#include "CameraDefaultBehavior.generated.h"

UCLASS()
class DIAMONDPROJECT_API UCameraDefaultBehavior : public UCameraBehavior {
	GENERATED_BODY()
	
public:
	virtual void BeginBehavior(ALuminariaCamera* Owner) override;
	
	UFUNCTION()
	void RegisterPlayer(ADiamondProjectCharacter* character);

	void OnPlayerMove(ADiamondProjectCharacter* character,FVector direction,bool& isCanceled);

	virtual void TickBehavior(float DeltaTime) override;
};
