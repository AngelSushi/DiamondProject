#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PlayerEventsDispatcher.generated.h"

class ADiamondProjectCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPlayerMove,ACharacter*,Character, FVector2D,Direction,bool,isCanceled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerRegister,ACharacter*,Character);
UCLASS()	
class DIAMONDPROJECT_API UPlayerEventsDispatcher : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	
	UPROPERTY(VisibleAnywhere)
	FOnPlayerMove OnPlayerMove;

	UPROPERTY(VisibleAnywhere)
	FOnPlayerRegister OnPlayerRegister;
	
};
