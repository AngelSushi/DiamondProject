// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatefomCPPTest.generated.h"

UCLASS()
class DIAMONDPROJECT_API AMovingPlatefomCPPTest : public AActor
{
	GENERATED_BODY()
	
public:	
	AMovingPlatefomCPPTest();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform Settings")
    TArray<AActor*> Waypoints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform Settings")
    float MovementSpeed;

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

private:
    FVector StartLocation;
    int32 CurrentWaypointIndex;
    bool bMovingForward;


};
