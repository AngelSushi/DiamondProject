// Fill out your copyright notice in the Description page of Project Settings.


#include "DiamondProject/Luminaria/Actors/AutoThwomp.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AAutoThwomp::AAutoThwomp()
{
    PrimaryActorTick.bCanEverTick = true;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    StaticMeshComponent->SetCollisionProfileName(TEXT("BlockAll"));
    StaticMeshComponent->SetupAttachment(RootComponent);
    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Overlap);

}

// Called when the game starts or when spawned
void AAutoThwomp::BeginPlay()
{
	Super::BeginPlay();
    StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AAutoThwomp::OnMeshBeginOverlap);
    InitialLocation = GetActorLocation();
}

// Called every frame
void AAutoThwomp::Tick(float DeltaTime)

{
	Super::Tick(DeltaTime);
    if(Up)
    {
        FVector ToTop = GetActorLocation();
        ToTop.Z += FallSpeed * DeltaTime;
        SetActorLocation(ToTop);
    }
    if (Down)
    {
        FVector ToBOT = GetActorLocation();
        ToBOT.Z -= FallSpeed * DeltaTime;
        SetActorLocation(ToBOT);
    }

}


void AAutoThwomp::OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!Up)
    {
        Up = true;
        Down = false;
    }
    if (Up)
    {
        Down = true;
        Up = false;
    }
}



