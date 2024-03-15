#pragma once

#include "CoreMinimal.h"
#include "CameraBehavior.h"
#include "GoToBehavior.generated.h"	


UCLASS()
class DIAMONDPROJECT_API UGoToBehavior : public UCameraBehavior {
	GENERATED_BODY()

public:
	UGoToBehavior();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	FVector Approach(FVector Current, FVector Target, float Incr);

};
