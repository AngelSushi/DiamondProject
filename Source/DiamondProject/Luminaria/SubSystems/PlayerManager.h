#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "../../../DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"
#include "../../../DiamondProject/Luminaria/Core/DiamondProjectPlayerController.h"
#include "PlayerManager.generated.h"

class ADiamondProjectCharacter;
class ACheckpoint;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnPlayerWalk,ADiamondProjectCharacter*,Character,FVector2D,Input,FVector,Direction,bool&,isCanceled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerMove, ADiamondProjectCharacter*, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerRegister,ADiamondProjectCharacter*,Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerDeath,ADiamondProjectCharacter*,Character,EDeathCause, DeathCause);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPlayerRespawn, ADiamondProjectCharacter*, Character, EDeathCause, DeathCause, FVector, RespawnPosition);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerUpdateCheckpoint, ADiamondProjectCharacter*, Character, ACheckpoint*, Checkpoint);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerLandOnGround, ADiamondProjectCharacter*, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeNewArea, ACameraArea*,Area);

UCLASS()	
class DIAMONDPROJECT_API UPlayerManager : public UTickableWorldSubsystem {
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere,BlueprintAssignable)
	FOnChangeNewArea OnChangeNewArea;
	
	UPROPERTY(VisibleAnywhere)
	FOnPlayerWalk OnPlayerWalk; // Called When The Player Pressed the Input To Walk

	UPROPERTY(VisibleAnywhere)
	FOnPlayerMove OnPlayerMove; // Called When The Player Moved (Include Input)

	UPROPERTY(VisibleAnywhere,BlueprintAssignable)
	FOnPlayerRegister OnPlayerRegister;

	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
	FOnPlayerDeath OnPlayerDeath;

	UPROPERTY(VisibleAnywhere,BlueprintAssignable)
	FOnPlayerRespawn OnPlayerRespawn;

	UPROPERTY(VisibleAnywhere,BlueprintAssignable)
	FOnPlayerUpdateCheckpoint OnPlayerUpdateCheckpoint;

	UPROPERTY(VisibleAnywhere,BlueprintAssignable)
	FOnPlayerLandOnGround OnPlayerLandOnGround;

	UFUNCTION()
	void RegisterPlayer(ADiamondProjectCharacter* Character);

	UFUNCTION(BlueprintCallable,BlueprintPure)
	ADiamondProjectCharacter* GetOnePlayer() { 
		if (GetAllCharactersRef().Num() > 0) {
			return GetAllCharactersRef()[0];
		}

		return nullptr;
	}

	UFUNCTION(BlueprintCallable)
	ADiamondProjectCharacter* GetOtherPlayer(ADiamondProjectCharacter* Character);

	UFUNCTION(BlueprintPure)
	TArray<ADiamondProjectCharacter*>& GetAllCharactersRef() { return Characters; }

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

	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> CinematicCharacters;

	UFUNCTION()
	TArray<int>& GetOrderedPlayers() { return OrderedPlayers; }

	virtual void Tick(float DeltaTime) override;
	
	virtual TStatId GetStatId() const override {
		return TStatId();
	}

private:
	TArray<int> OrderedPlayers;
};
