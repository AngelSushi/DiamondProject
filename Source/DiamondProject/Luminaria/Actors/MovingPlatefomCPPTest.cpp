#include "DiamondProject/Luminaria/Actors/MovingPlatefomCPPTest.h"
#include "DiamondProject/Luminaria/SubSystems/MecanismEventsDispatcher.h"
#include "DiamondProject/Luminaria/DataAssets/PlateformDataAsset.h"

#include "Components/BoxComponent.h"

AMovingPlatefomCPPTest::AMovingPlatefomCPPTest(){
    PrimaryActorTick.bCanEverTick = true;

    Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
    Cube->SetupAttachment(RootComponent);

    BoxBlock = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxBlock"));
    BoxBlock->SetupAttachment(Cube);

    BoxOverlap = CreateDefaultSubobject<UBoxComponent>(TEXT("BlockOverlap"));
    BoxOverlap->SetupAttachment(Cube);
}

void AMovingPlatefomCPPTest::BeginPlay() {
    Super::BeginPlay();

    StartLocation = GetActorLocation();
    CurrentWaypointIndex = 0;
    bMovingForward = true;

    UMecanismEventsDispatcher* MecanismDispatcher = GetWorld()->GetSubsystem<UMecanismEventsDispatcher>();

    MecanismDispatcher->OnMecanismOn.AddDynamic(this, &AMovingPlatefomCPPTest::OnMecanismOn);
    MecanismDispatcher->OnMecanismOff.AddDynamic(this, &AMovingPlatefomCPPTest::OnMecanismOff);

    for (int i = 0; i < 4; i++) {
        UMaterialInstanceDynamic* InstanceDynamic = UMaterialInstanceDynamic::Create(CrystalMaterialRef, this);
        InstanceDynamic->SetScalarParameterValue("BlendAlpha", 0.F);
        CrystalsMat.Add(InstanceDynamic);
        Cube->SetMaterial(5 + i, InstanceDynamic);
    }

    BoxOverlap->OnComponentBeginOverlap.AddDynamic(this, &AMovingPlatefomCPPTest::OnBeginOverlap);
    BoxOverlap->OnComponentEndOverlap.AddDynamic(this, &AMovingPlatefomCPPTest::OnEndOverlap);
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

void AMovingPlatefomCPPTest::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
    GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Green, TEXT("Begin Overlap"));

    PlayerOn++;
    PlayerOn = FMath::Clamp(PlayerOn,0, 2);

    int PlayerMaxIndex = PlayerOn * 2;

    for (int i = 0; i < PlayerMaxIndex; i++) {
        UMaterialInstanceDynamic* InstanceMaterial = CrystalsMat[i];
        InstanceMaterial->SetScalarParameterValue("BlendAlpha", 1.0F);
    }
}

void AMovingPlatefomCPPTest::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
    GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Red, TEXT("End Overlap"));

    PlayerOn--;
    PlayerOn = FMath::Clamp(PlayerOn, 0, 2);

    int PlayerMaxIndex = PlayerOn * 2;


    if (PlayerOn == 0) {
        for (UMaterialInstanceDynamic* InstanceMaterial : CrystalsMat) {
            InstanceMaterial->SetScalarParameterValue("BlendAlpha", 0.F);
        }
        return;
    }

    for (int i = 0; i < PlayerMaxIndex; i++) {
        int Index = 3 - i;
        UMaterialInstanceDynamic* InstanceMaterial = CrystalsMat[Index];
        InstanceMaterial->SetScalarParameterValue("BlendAlpha", 0.0F);
    }
}