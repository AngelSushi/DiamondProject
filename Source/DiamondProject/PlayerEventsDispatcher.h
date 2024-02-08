#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PlayerEventsDispatcher.generated.h"

class ADiamondProjectCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPlayerMove,ACharacter*,Character, FVector2D,Direction,bool,isCanceled);

UCLASS()	
class DIAMONDPROJECT_API UPlayerEventsDispatcher : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	
	UPROPERTY(VisibleAnywhere)
	FOnPlayerMove OnPlayerMove;
	
};
