#pragma once

#include "CoreMinimal.h"
#include "CameraBehavior.h"
#include "DebugCameraBehaviorComponent.generated.h"


UCLASS()
class DIAMONDPROJECT_API UDebugCameraBehaviorComponent : public UCameraBehavior
{
	GENERATED_BODY()

public:
	UDebugCameraBehaviorComponent();

	UPROPERTY(EditAnywhere)
	TArray<UActorComponent*> ComponentsList;

private:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
};
