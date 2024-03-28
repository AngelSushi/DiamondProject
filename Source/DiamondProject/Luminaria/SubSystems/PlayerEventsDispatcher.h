#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PlayerEventsDispatcher.generated.h"

class ADiamondProjectCharacter;
class ACheckpoint;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPlayerMove,ADiamondProjectCharacter*,Character, FVector,Direction,bool&,isCanceled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerRegister,ADiamondProjectCharacter*,Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDeath,ADiamondProjectCharacter*,Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerUpdateCheckpoint, ADiamondProjectCharacter*, Character, ACheckpoint*, Checkpoint);

UCLASS()	
class DIAMONDPROJECT_API UPlayerEventsDispatcher : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	
	UPROPERTY(VisibleAnywhere)
	FOnPlayerMove OnPlayerMove;

	UPROPERTY(VisibleAnywhere)
	FOnPlayerRegister OnPlayerRegister;

	UPROPERTY(VisibleAnywhere)
	FOnPlayerDeath OnPlayerDeath;

	UPROPERTY(VisibleAnywhere)
	FOnPlayerUpdateCheckpoint OnPlayerUpdateCheckpoint;
};
