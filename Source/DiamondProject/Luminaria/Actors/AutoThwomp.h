// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AutoThwomp.generated.h"

UCLASS()
class DIAMONDPROJECT_API AAutoThwomp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAutoThwomp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
    
    UPROPERTY(EditAnywhere)
    class UStaticMeshComponent* StaticMeshComponent;

    UPROPERTY(VisibleAnywhere)
    bool Up = false;

    UPROPERTY(VisibleAnywhere)
    bool Down = true;

    UPROPERTY(VisibleAnywhere)
    bool isFunctionAlreadyCalled = false;

    FVector InitialLocation;

    UPROPERTY(EditAnywhere)
    float FallSpeed = 100.0f;

    UPROPERTY(EditAnywhere)
    float BackSpeed = 100.0f;

    UPROPERTY(EditAnywhere)
    TObjectPtr<class UDeathComponent> DeathComponent;

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


    UFUNCTION()
    void ChangeDirection();

};
