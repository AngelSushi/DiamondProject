#include "DiamondProject/Luminaria/Actors/MovingPlatefomCPPTest.h"

AMovingPlatefomCPPTest::AMovingPlatefomCPPTest()
{
    PrimaryActorTick.bCanEverTick = true;
    MovementSpeed = 100.0f;

}

void AMovingPlatefomCPPTest::BeginPlay()
{
    Super::BeginPlay();

    StartLocation = GetActorLocation();
    CurrentWaypointIndex = 0;
    bMovingForward = true;
	
}


void AMovingPlatefomCPPTest::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (Waypoints.Num() == 0)
        return;

    FVector TargetLocation = Waypoints[CurrentWaypointIndex]->GetActorLocation();
    FVector CurrentLocation = GetActorLocation();

    FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
    FVector NewLocation = CurrentLocation + Direction * MovementSpeed * DeltaTime;

    SetActorLocation(NewLocation);

    float DistanceSquared = FVector::DistSquared(CurrentLocation, TargetLocation);
    if (DistanceSquared <= FMath::Square(10.0f)) // Distance de tolérance
    {
        if (bMovingForward)
        {
            CurrentWaypointIndex++;
            if (CurrentWaypointIndex >= Waypoints.Num())
            {
                CurrentWaypointIndex = Waypoints.Num() - 2;
                bMovingForward = false;
            }
        }
        else
        {
            CurrentWaypointIndex--;
            if (CurrentWaypointIndex < 0)
            {
                CurrentWaypointIndex = 1;
                bMovingForward = true;
            }
        }
    }
}
