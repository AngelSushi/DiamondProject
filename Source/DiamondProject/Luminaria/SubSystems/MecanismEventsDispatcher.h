#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MecanismEventsDispatcher.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMecanismOn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMecanismOff);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMecanismActivate,AMecanism*,mecanism,AMecanismActivator*, mecanismActivator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMecanismDeactivate,AMecanism*,mecanism,AMecanismActivator*, mecanismActivator);

UCLASS()
class DIAMONDPROJECT_API UMecanismEventsDispatcher : public UWorldSubsystem {
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere,BlueprintAssignable)
	FOnMecanismOn OnMecanismOn;

	UPROPERTY(VisibleAnywhere,BlueprintAssignable)
	FOnMecanismOff OnMecanismOff;

	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
	FOnMecanismActivate OnMecanismActivate;

	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
	FOnMecanismDeactivate OnMecanismDeactivate;
	
};
