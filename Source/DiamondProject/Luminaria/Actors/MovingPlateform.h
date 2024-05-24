// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlateform.generated.h"

UCLASS()
class DIAMONDPROJECT_API AMovingPlateform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingPlateform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	int32 NumberOfComponents;
private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TArray<USceneComponent*> SceneComponents;

	void AddComponents();
};


};
