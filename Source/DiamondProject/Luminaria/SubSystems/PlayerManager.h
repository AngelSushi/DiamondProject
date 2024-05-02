#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PlayerManager.generated.h"

class ADiamondProjectCharacter;
class ACheckpoint;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPlayerMove,ADiamondProjectCharacter*,Character, FVector,Direction,bool&,isCanceled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerRegister,ADiamondProjectCharacter*,Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDeath,ADiamondProjectCharacter*,Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerUpdateCheckpoint, ADiamondProjectCharacter*, Character, ACheckpoint*, Checkpoint);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerLandOnGround, ADiamondProjectCharacter*, Character);

UCLASS()	
class DIAMONDPROJECT_API UPlayerManager : public UWorldSubsystem {
	GENERATED_BODY()

public:
	
	UPROPERTY(VisibleAnywhere)
	FOnPlayerMove OnPlayerMove;

	UPROPERTY(VisibleAnywhere)
	FOnPlayerRegister OnPlayerRegister;

	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
	FOnPlayerDeath OnPlayerDeath;

	UPROPERTY(VisibleAnywhere)
	FOnPlayerUpdateCheckpoint OnPlayerUpdateCheckpoint;

	UPROPERTY(VisibleAnywhere)
	FOnPlayerLandOnGround OnPlayerLandOnGround;

	UFUNCTION()
	void RegisterPlayer(ADiamondProjectCharacter* Character);

	UFUNCTION()
	ADiamondProjectCharacter* GetOtherPlayer(ADiamondProjectCharacter* Character);

	UPROPERTY()
	TArray<ADiamondProjectCharacter*> Characters;
};
