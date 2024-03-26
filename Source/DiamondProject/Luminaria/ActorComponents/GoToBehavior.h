#pragma once

#include "CoreMinimal.h"
#include "CameraBehavior.h"
#include "../Actors/LuminariaCamera.h"
#include "GoToBehavior.generated.h"	


UCLASS()
class DIAMONDPROJECT_API UGoToBehavior : public UCameraBehavior {
	GENERATED_BODY()

public:
	UGoToBehavior();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector GoTo;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Speed;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<ECameraBehavior> NextBehavior;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	bool Approach(FVector& Current, FVector Target, float Incr);

};
