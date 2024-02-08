// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbsorbLightEnemy.generated.h"

UCLASS()
class DIAMONDPROJECT_API AAbsorbLightEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbsorbLightEnemy();

	UPROPERTY(VisibleAnywhere, Category = "Default")
		TObjectPtr<UPointLightComponent> PointLight;

	UPROPERTY(VisibleAnywhere, Category = "Default")
		TObjectPtr<UBoxComponent> TriggerZone;

	UPROPERTY( VisibleAnywhere, Category = "Default")
		TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, Category = "BP_absorbLight")
		TObjectPtr<UTimelineComponent> AbsorbTime;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
		double damage;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
		bool isAbsorb;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
