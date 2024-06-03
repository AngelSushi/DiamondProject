#include "DiamondProject/Luminaria/Actors/MovingPlatefomCPPTest.h"
#include "DiamondProject/Luminaria/SubSystems/MecanismEventsDispatcher.h"
#include "DiamondProject/Luminaria/DataAssets/PlateformDataAsset.h"

AMovingPlatefomCPPTest::AMovingPlatefomCPPTest(){
    PrimaryActorTick.bCanEverTick = true;
}

void AMovingPlatefomCPPTest::BeginPlay()
{
    Super::BeginPlay();

    StartLocation = GetActorLocation();
    CurrentWaypointIndex = 0;
    bMovingForward = true;

    UMecanismEventsDispatcher* MecanismDispatcher = GetWorld()->GetSubsystem<UMecanismEventsDispatcher>();

    MecanismDispatcher->OnMecanismOn.AddDynamic(this, &AMovingPlatefomCPPTest::OnMecanismOn);
    MecanismDispatcher->OnMecanismOff.AddDynamic(this, &AMovingPlatefomCPPTest::OnMecanismOff);
}


void AMovingPlatefomCPPTest::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    if (Waypoints.Num() == 0)
        return;

    if (!TargetMecanism || (TargetMecanism && bEnable)) {

        FVector TargetLocation = Waypoints[CurrentWaypointIndex]->GetActorLocation();
        FVector CurrentLocation = GetActorLocation();

        FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
        FVector NewLocation = CurrentLocation + Direction * GetPlateformAsset()->Speed * DeltaTime;

        SetActorLocation(NewLocation);

        float DistanceSquared = FVector::DistSquared(CurrentLocation, TargetLocation);
        if (DistanceSquared <= FMath::Square(10.0f)) // Distance de tolérance
        {
            if (bMovingForward) {
                CurrentWaypointIndex++;
                if (CurrentWaypointIndex >= Waypoints.Num()) {
                    CurrentWaypointIndex = Waypoints.Num() - 2;
                    bMovingForward = false;
                }
            }
            else {
                CurrentWaypointIndex--;
                if (CurrentWaypointIndex < 0) {
                    CurrentWaypointIndex = 1;
                    bMovingForward = true;
                }
            }
        }
    }
}

void AMovingPlatefomCPPTest::OnMecanismOn(AMecanism* Mecanism) {
    if (Mecanism == TargetMecanism) {
        bEnable = true;
    }
}

void AMovingPlatefomCPPTest::OnMecanismOff(AMecanism* Mecanism) {
    if (Mecanism == TargetMecanism) {
        bEnable = false;
    }
}
