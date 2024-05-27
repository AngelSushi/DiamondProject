// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MecanismRewardActor.h"
#include "MovingPlatefomCPPTest.generated.h"

UCLASS()
class DIAMONDPROJECT_API AMovingPlatefomCPPTest : public AMecanismRewardActor {
	GENERATED_BODY()
	
public:	
	AMovingPlatefomCPPTest();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform Settings")
    TArray<AActor*> Waypoints;

  

    UFUNCTION(BlueprintPure)
    UPlateformDataAsset* GetPlateformAsset() { return PlateformAsset; }

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

private:
    UPROPERTY(EditAnywhere, Category = "Platform Settings")
    TObjectPtr<class UPlateformDataAsset> PlateformAsset;

    FVector StartLocation;
    int32 CurrentWaypointIndex;
    bool bMovingForward;

    UPROPERTY()
    bool bEnable;

    void OnMecanismOn(AMecanism* Mecanism);
    void OnMecanismOff(AMecanism* Mecanism);


};
