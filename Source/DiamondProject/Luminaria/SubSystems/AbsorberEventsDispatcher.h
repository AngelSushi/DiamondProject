#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "AbsorberEventsDispatcher.generated.h"

class ADiamondProjectCharacter;
class AAbsorber;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDetectPlayer, ADiamondProjectCharacter*, Character, AAbsorber*, Absorber);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUnDetectPlayer, ADiamondProjectCharacter*, Character, AAbsorber*, Absorber);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStunAbsorber, AAbsorber*, Absorber);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeStunAbsorber, AAbsorber*, Absorber);


UCLASS()
class DIAMONDPROJECT_API UAbsorberEventsDispatcher : public UWorldSubsystem {
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
	FOnDetectPlayer OnDetectPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
	FOnUnDetectPlayer OnUnDetectPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
	FOnStunAbsorber OnStunAbsorber;

	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
	FOnDeStunAbsorber OnDeStunAbsorber;
	
};
