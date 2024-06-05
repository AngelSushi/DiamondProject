// Fill out your copyright notice in the Description page of Project Settings.


#include "DiamondProject/Luminaria/Actors/Thwomp.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"
#include "DiamondProject/Luminaria/ActorComponents/DeathComponent.h"

// Sets default values
AThwomp::AThwomp()
{
    PrimaryActorTick.bCanEverTick = true;

    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    RootComponent = BoxComponent;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    StaticMeshComponent->SetCollisionProfileName(TEXT("BlockAll"));
    StaticMeshComponent->SetupAttachment(RootComponent);

    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); 
    StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Overlap);

    DeathComponent = CreateDefaultSubobject<UDeathComponent>(TEXT("Death Component"));
    DeathComponent->SetupAttachment(RootComponent);

    bIsShaking = false;
    ShakeDuration = 2.0f;
    ShakeTime = 0.0f;

}
void AThwomp::BeginPlay()
{
    Super::BeginPlay();

    BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AThwomp::OnOverlapBegin);
    StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AThwomp::OnMeshBeginOverlap);
    StaticMeshComponent->OnComponentEndOverlap.AddDynamic(this, &AThwomp::OnMeshEndOverlap);
    InitialLocation = GetActorLocation();
}

void AThwomp::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsShaking)
    {
        ShakeTime += DeltaTime;
        if (ShakeTime >= ShakeDuration)
        {
            bIsShaking = false;
            bIsFalling = true;
            ShakeTime = 0.0f;
        }
        else
        {
            ShakeBox();
        }
    }

    if (bIsFalling && !bIsTouchingSomething)
    {
        FVector NewLocation = GetActorLocation();
        NewLocation.Z -= FallSpeed * DeltaTime;
        SetActorLocation(NewLocation);
    }

    if (bIsFalling)
    {
        CurrentReturnDelay += DeltaTime;
        if (CurrentReturnDelay >= ReturnDelay || bIsTouchingSomething)
        {
            
            bIsFalling = false;
            CurrentReturnDelay = 0;
        }
    }
    
    if (!bIsFalling)
    {
        if ((GetActorLocation().Z <= InitialLocation.Z))
        {
            FVector NewBaseLocation = GetActorLocation();
            NewBaseLocation.Z += BackSpeed * DeltaTime;
            SetActorLocation(NewBaseLocation);
        }
        
    }


}
void AThwomp::ShakeBox()
{
    FVector NewLocation = InitialLocation;
    NewLocation.X += FMath::RandRange(-5.0f, 5.0f);
    NewLocation.Y += FMath::RandRange(-5.0f, 5.0f);
    SetActorLocation(NewLocation);
}


void AThwomp::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ADiamondProjectCharacter* Character = Cast<ADiamondProjectCharacter>(OtherActor);
    if (Character)
    {
        bIsShaking = true;
        //bIsFalling = true;
    }
}

void AThwomp::OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    bIsTouchingSomething = true;
}

void AThwomp::OnMeshEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    bIsTouchingSomething = false;
}