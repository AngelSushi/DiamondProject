#pragma once

#include "CoreMinimal.h"
#include "CameraBehavior.h"
#include "../Actors/LuminariaCamera.h"
#include "GoToBehavior.generated.h"	


UCLASS()
class DIAMONDPROJECT_API UGoToBehavior : public UCameraBehavior {
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector GoTo;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<ECameraBehavior> NextBehavior;

	virtual void BeginBehavior(ALuminariaCamera* Owner) override;
	virtual void TickBehavior(float DeltaTime) override;

private:
	bool Approach(FVector& Current, FVector Target, float Incr);

	UPROPERTY()
	float CurrentTimer;

	UPROPERTY()
	float MaxTimer;
};
