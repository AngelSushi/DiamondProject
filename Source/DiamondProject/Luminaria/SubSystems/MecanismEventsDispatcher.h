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
	UPROPERTY(VisibleAnywhere)
	FOnMecanismOn OnMecanismOn;

	UPROPERTY(VisibleAnywhere)
	FOnMecanismOff OnMecanismOff;

	UPROPERTY(VisibleAnywhere)
	FOnMecanismActivate OnMecanismActivate;

	UPROPERTY(VisibleAnywhere)
	FOnMecanismDeactivate OnMecanismDeactivate;
	
};
