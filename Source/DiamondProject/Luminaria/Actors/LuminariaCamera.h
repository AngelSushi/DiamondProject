#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "LuminariaCamera.generated.h"

UENUM(BlueprintType)
enum ECameraBehavior : uint8
{
	DEFAULT,
	LEADER,
	DYNAMIC,
};

UCLASS()
class DIAMONDPROJECT_API ALuminariaCamera : public ACameraActor
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TEnumAsByte<ECameraBehavior> CameraBehavior;
	
};
