// Fill out your copyright notice in the Description page of Project Settings.


#include "DiamondProject/Luminaria/Actors/AutoThwomp.h"
#include "Components/StaticMeshComponent.h"
#include "DiamondProject/Luminaria/ActorComponents/DeathComponent.h"



// Sets default values
AAutoThwomp::AAutoThwomp()
{
    PrimaryActorTick.bCanEverTick = true;


    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    StaticMeshComponent->SetCollisionProfileName(TEXT("BlockAll"));
    SetRootComponent(StaticMeshComponent);


    StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Overlap);

    DeathComponent = CreateDefaultSubobject<UDeathComponent>(TEXT("Death Component"));


}

// Called when the game starts or when spawned
void AAutoThwomp::BeginPlay()
{
	Super::BeginPlay();
    StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AAutoThwomp::OnMeshBeginOverlap);
    StaticMeshComponent->OnComponentEndOverlap.AddDynamic(this, &AAutoThwomp::OnMeshEndOverlap);
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

void AAutoThwomp::ChangeDirection() 
{
    if (!Up)
    {
        Up = true;
        Down = false;

    }
    else if (Up)
    {
        Down = true;
        Up = false;

    }

}


void AAutoThwomp::OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!isFunctionAlreadyCalled) 
    {
        ChangeDirection();
        isFunctionAlreadyCalled = true;
        
    }
}

void AAutoThwomp::OnMeshEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    isFunctionAlreadyCalled = false;
}

