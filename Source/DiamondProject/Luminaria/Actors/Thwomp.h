// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Thwomp.generated.h"

UCLASS()
class DIAMONDPROJECT_API AThwomp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThwomp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
    UPROPERTY(EditAnywhere)
    class UBoxComponent* BoxComponent;

    UPROPERTY(EditAnywhere)
    class UStaticMeshComponent* StaticMeshComponent;


    UPROPERTY(VisibleAnywhere)
    bool bIsFalling = false;

    UPROPERTY(VisibleAnywhere)
    bool bIsTouchingSomething = false;

    FVector InitialLocation;

    UPROPERTY(EditAnywhere)
    float FallSpeed = 100.0f;

    UPROPERTY(EditAnywhere)
    float BackSpeed = 100.0f;

    UPROPERTY(EditAnywhere)
    float ReturnDelay = 2.0f; 

    UPROPERTY(VisibleAnywhere)
    float CurrentReturnDelay = 0.0f;

    UPROPERTY(EditAnywhere)
    TObjectPtr<class UDeathComponent> DeathComponent;


    void ShakeBox();

    bool bIsShaking;

    float ShakeTime;

    UPROPERTY(EditAnywhere)
    float ShakeDuration;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

    UFUNCTION()
    void OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

    UFUNCTION()
    void OnMeshEndOverlap(UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex);
};
