#pragma once

#include "CoreMinimal.h"
#include "CameraBehavior.h"
#include "CameraShakeBehavior.generated.h"


UCLASS()
class DIAMONDPROJECT_API UCameraShakeBehavior : public UCameraBehavior {
	GENERATED_BODY()
	
public:
	virtual void BeginBehavior(ALuminariaCamera* Owner);
	virtual void TickBehavior(float DeltaTime);

};
