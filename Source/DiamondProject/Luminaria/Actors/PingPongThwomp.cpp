#include "DiamondProject/Luminaria/Actors/PingPongThwomp.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "DiamondProject/Luminaria/ActorComponents/DeathComponent.h"


APingPongThwomp::APingPongThwomp()
{
    PrimaryActorTick.bCanEverTick = true;

    // Créer le composant de forme statique et le configurer comme un cube
    CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
    SetRootComponent(CubeMesh);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")); // Charger un cube prédéfini dans Unreal Engine
    if (CubeMeshAsset.Succeeded())
    {
        CubeMesh->SetStaticMesh(CubeMeshAsset.Object);
    }
    Speed = 100.f;


    StartWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("StartWidgetComponent"));
    StartWidgetComponent->SetupAttachment(RootComponent);
    StartWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f)); // Position au-dessus du cube
    StartWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
    StartWidgetComponent->SetDrawSize(FVector2D(50.f, 50.f)); // Taille du widget

    // Créer et configurer le composant de widget pour le point d'arrivée
    EndWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("EndWidgetComponent"));
    EndWidgetComponent->SetupAttachment(RootComponent);
    EndWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f)); // Position au-dessus du cube
    EndWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
    EndWidgetComponent->SetDrawSize(FVector2D(50.f, 50.f));
    

    DeathComponent = CreateDefaultSubobject<UDeathComponent>(TEXT("Death Component"));
    DeathComponent->SetupAttachment(RootComponent);
}

void APingPongThwomp::BeginPlay()
{
    Super::BeginPlay();
    StartPoint = StartWidgetComponent->GetComponentLocation();
    EndPoint = EndWidgetComponent->GetComponentLocation();
    // Initialize start and end points
    CurrentTarget = EndPoint;
}

void APingPongThwomp::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Move towards the target
    FVector CurrentLocation = GetActorLocation();
    FVector Direction = (CurrentTarget - CurrentLocation).GetSafeNormal();
    FVector NewLocation = CurrentLocation + Direction * Speed * DeltaTime;
    SetActorLocation(NewLocation);

    // Check if reached target
    if (FVector::DistSquared(CurrentLocation, CurrentTarget) < 1.0f)
    {
        // Swap target between start and end points
        if (CurrentTarget.Equals(StartPoint))
            CurrentTarget = EndPoint;
        else
            CurrentTarget = StartPoint;
    }

    StartWidgetComponent->SetWorldLocation(StartPoint);
    EndWidgetComponent->SetWorldLocation(EndPoint);
}

