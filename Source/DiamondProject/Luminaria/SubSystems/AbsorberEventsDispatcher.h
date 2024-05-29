#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "AbsorberEventsDispatcher.generated.h"

class ADiamondProjectCharacter;
class AAbsorber;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDetectPlayer, ADiamondProjectCharacter*, Character, AAbsorber*, Absorber);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUnDetectPlayer, ADiamondProjectCharacter*, Character, AAbsorber*, Absorber);

UCLASS()
class DIAMONDPROJECT_API UAbsorberEventsDispatcher : public UWorldSubsystem {
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	FOnDetectPlayer OnDetectPlayer;

	UPROPERTY(VisibleAnywhere)
	FOnUnDetectPlayer OnUnDetectPlayer;
	
};
