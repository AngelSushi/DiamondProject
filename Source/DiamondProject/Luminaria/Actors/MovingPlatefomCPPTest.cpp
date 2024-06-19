     #include "DiamondProject/Luminaria/Actors/MovingPlatefomCPPTest.h"
#include "DiamondProject/Luminaria/SubSystems/MecanismEventsDispatcher.h"
#include "DiamondProject/Luminaria/DataAssets/PlateformDataAsset.h"

#include "Components/BoxComponent.h"

#include "../Core/DiamondProjectCharacter.h"

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

    bIsGroundPlateform = Cube->GetMaterials().Num() == 8;

    if (!bIsGroundPlateform) {
        for (int i = 0; i < 4; i++) {
            UMaterialInstanceDynamic* InstanceDynamic = UMaterialInstanceDynamic::Create(CrystalMaterialRef, this);
            InstanceDynamic->SetScalarParameterValue("BlendAlpha", 0.F);
            CrystalsMat.Add(InstanceDynamic);
            Cube->SetMaterial(5 + i, InstanceDynamic);
        }
    }
    else {
        UMaterialInstanceDynamic* InstanceDynamic = UMaterialInstanceDynamic::Create(CrystalMaterialRef, this);
        InstanceDynamic->SetScalarParameterValue("Value", 0.F);
        CrystalsMat.Add(InstanceDynamic);
        Cube->SetMaterial(1, InstanceDynamic);
    }

    BoxOverlap->OnComponentBeginOverlap.AddDynamic(this, &AMovingPlatefomCPPTest::OnBeginOverlap);
    BoxOverlap->OnComponentEndOverlap.AddDynamic(this, &AMovingPlatefomCPPTest::OnEndOverlap);
}



void AMovingPlatefomCPPTest::OnMecanismOn(AMecanism* Mecanism) {
    if (TargetMecanisms.Contains(Mecanism)) {
        bEnable = true;
    }
}

void AMovingPlatefomCPPTest::OnMecanismOff(AMecanism* Mecanism) {
    if (TargetMecanisms.Contains(Mecanism)) {
        bEnable = false;
    }
}

void AMovingPlatefomCPPTest::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
    if (ADiamondProjectCharacter* Character = Cast<ADiamondProjectCharacter>(OtherActor)) {
        if (!CrystalsColor.Contains(Character->GetPlayerColor())) {
            CrystalsColor.Add(Character->GetPlayerColor());
        }

        PlayerOn++;
        PlayerOn = FMath::Clamp(PlayerOn, 0, 2);

        if (!bIsGroundPlateform) {
            int PlayerMaxIndex = PlayerOn * 2;

            for (int i = 0; i < PlayerMaxIndex; i++) {
                int ColorIndex = i / 2;
                UMaterialInstanceDynamic* InstanceMaterial = CrystalsMat[i];
                
                InstanceMaterial->SetScalarParameterValue("BlendAlpha", 1.0F);
                InstanceMaterial->SetVectorParameterValue("Color", CrystalsColor[ColorIndex]);
            }
        }
        else {
            UMaterialInstanceDynamic* InstanceMaterial = CrystalsMat[0];

            float Value = 1.F - 0.25F * (PlayerOn / 2);

            if (PlayerOn % 2 == 0) { // If The Two Players Are On The Plateform
                InstanceMaterial->SetVectorParameterValue("FirstColor", CrystalsColor[0]);
                InstanceMaterial->SetVectorParameterValue("SecondColor", CrystalsColor[1]);
            }
            else {
                InstanceMaterial->SetVectorParameterValue("FirstColor", CrystalsColor[0]);
                InstanceMaterial->SetVectorParameterValue("SecondColor", CrystalsColor[0]);
            }

            InstanceMaterial->SetScalarParameterValue("Value", Value);
        }
    }
    
}

void AMovingPlatefomCPPTest::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
    if (ADiamondProjectCharacter* Character = Cast<ADiamondProjectCharacter>(OtherActor)) {
        PlayerOn--;
        PlayerOn = FMath::Clamp(PlayerOn, 0, 2);

        if (CrystalsColor.Contains(Character->GetPlayerColor())) {
            CrystalsColor.Remove(Character->GetPlayerColor());
        }

        if (!bIsGroundPlateform) {
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
        else {
            UMaterialInstanceDynamic* InstanceMaterial = CrystalsMat[0];

            InstanceMaterial->SetScalarParameterValue("Value", 1.F);

            if (PlayerOn == 0) {
                InstanceMaterial->SetVectorParameterValue("FirstColor",FColor::Black);
                InstanceMaterial->SetVectorParameterValue("SecondColor",FColor::Black);
            }
            else {
                InstanceMaterial->SetVectorParameterValue("FirstColor",CrystalsColor[0]);
                InstanceMaterial->SetVectorParameterValue("SecondColor",CrystalsColor[0]);
            }
        }
    }
}