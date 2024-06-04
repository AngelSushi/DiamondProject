#pragma once

#include "CoreMinimal.h"
#include "CameraBehavior.h"
#include "CameraFollowBehavior.generated.h"

class UCameraBehavior;

UCLASS()
class DIAMONDPROJECT_API UCameraFollowBehavior : public UCameraBehavior {
	GENERATED_BODY()

public:

	virtual void BeginBehavior(ALuminariaCamera* Owner) override;
	virtual void TickBehavior(float DeltaTime) override;

private:
	UPROPERTY()
	float DistanceToSpline;

	UFUNCTION()
	void OnRegisterPlayer(ADiamondProjectCharacter* Character);
};
