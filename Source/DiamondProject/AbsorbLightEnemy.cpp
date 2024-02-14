// Fill out your copyright notice in the Description page of Project Settings.


#include "AbsorbLightEnemy.h"
#include "Components/BoxComponent.h"

// Sets default values
AAbsorbLightEnemy::AAbsorbLightEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

/*	damage = 5000;
	isAbsorb = false;

	TriggerZone = CreateDefaultSubobject<UBoxComponent>(FName("TriggerZone"));
	TriggerZone->InitBoxSize(1f, 1f, 1f);


	TriggerZone->OnComponentBeginOverlap.AddDynamic(this, &AAbsorbLightEnemy::OnOverlapBegin);
*/
}

// Called when the game starts or when spawned
void AAbsorbLightEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAbsorbLightEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAbsorbLightEnemy::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
//		print("lalalalala");
	}
}

