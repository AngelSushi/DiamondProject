#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "../../../DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"
#include "../../../DiamondProject/Luminaria/Core/DiamondProjectPlayerController.h"
#include "PlayerManager.generated.h"

class ADiamondProjectCharacter;
class ADiamondProjectPlayerController;
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

	UPROPERTY(VisibleAnywhere,BlueprintAssignable)
	FOnPlayerRegister OnPlayerRegister;

	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
	FOnPlayerDeath OnPlayerDeath;

	UPROPERTY(VisibleAnywhere)
	FOnPlayerRespawn OnPlayerRespawn;

	UPROPERTY(VisibleAnywhere,BlueprintAssignable)
	FOnPlayerUpdateCheckpoint OnPlayerUpdateCheckpoint;

	UPROPERTY(VisibleAnywhere)
	FOnPlayerLandOnGround OnPlayerLandOnGround;

	UFUNCTION()
	void RegisterPlayer(ADiamondProjectCharacter* Character);

	UFUNCTION(BlueprintCallable)
	ADiamondProjectCharacter* GetOtherPlayer(ADiamondProjectCharacter* Character);

	UFUNCTION(BlueprintCallable)
	TArray<ADiamondProjectCharacter*> GetAllCharactersRef() { return Characters; }

	UFUNCTION(BlueprintCallable)
	TArray<ADiamondProjectPlayerController*> GetAllControllersRef() {
		TArray<ADiamondProjectPlayerController*> Controllers;

		for (ADiamondProjectCharacter* Character : GetAllCharactersRef()) {
			if (ADiamondProjectPlayerController* PlayerController = Cast<ADiamondProjectPlayerController>(Character->GetController())) {
				Controllers.Add(PlayerController);
			}
		}

		return Controllers;
	}

	UPROPERTY() // PASSER SA EN PRIVE
	TArray<ADiamondProjectCharacter*> Characters;
};
