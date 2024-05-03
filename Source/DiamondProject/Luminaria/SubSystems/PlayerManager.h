#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "../../../DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"
#include "PlayerManager.generated.h"

class ADiamondProjectCharacter;
class ACheckpoint;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPlayerMove,ADiamondProjectCharacter*,Character, FVector,Direction,bool&,isCanceled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerRegister,ADiamondProjectCharacter*,Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerDeath,ADiamondProjectCharacter*,Character,EDeathCause, DeathCause);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPlayerRespawn, ADiamondProjectCharacter*, Character, EDeathCause, DeathCause, FVector, RespawnPosition);
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
	FOnPlayerRespawn OnPlayerRespawn;

	UPROPERTY(VisibleAnywhere)
	FOnPlayerUpdateCheckpoint OnPlayerUpdateCheckpoint;

	UPROPERTY(VisibleAnywhere)
	FOnPlayerLandOnGround OnPlayerLandOnGround;

	UFUNCTION()
	void RegisterPlayer(ADiamondProjectCharacter* Character);

	UFUNCTION()
	ADiamondProjectCharacter* GetOtherPlayer(ADiamondProjectCharacter* Character);

	UFUNCTION()
	TArray<ADiamondProjectCharacter*> GetAllCharactersRef() { return Characters; }

	UPROPERTY() // PASSER SA EN PRIVE
	TArray<ADiamondProjectCharacter*> Characters;
};
