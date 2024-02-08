#pragma once

#include "CoreMinimal.h"
#include "CameraBehavior.h"
#include "CameraLeaderBehavior.generated.h"

UCLASS()
class DIAMONDPROJECT_API UCameraLeaderBehavior : public UCameraBehavior
{
	GENERATED_BODY()

	UCameraLeaderBehavior();
	
protected:
	virtual void BeginPlay() override;
	
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	TObjectPtr<class ACharacter> Leader;
	
};
