// Fill out your copyright notice in the Description page of Project Settings.


#include "DiamondProject/Luminaria/Actors/MovingPlateform.h"

// Sets default values
AMovingPlateform::AMovingPlateform()
{
	PrimaryActorTick.bCanEverTick = true;
	NumberOfComponents = 0;

}

// Called when the game starts or when spawned
void AMovingPlateform::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMovingPlateform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMovingPlateform::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Ajouter les composants dans l'éditeur
	AddComponents();
}
void AMovingPlateform::AddComponents()
{
    // Supprimer les anciens composants s'ils existent
    for (USceneComponent* Component : SceneComponents)
    {
        if (Component)
        {
            Component->DestroyComponent();
        }
    }
    SceneComponents.Empty();

    // Ajouter de nouveaux composants en fonction de la valeur de NumberOfComponents
    for (int32 i = 0; i < NumberOfComponents; ++i)
    {
        USceneComponent* NewComponent = NewObject<USceneComponent>(this);
        if (NewComponent)
        {
            NewComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
            NewComponent->RegisterComponent();
            SceneComponents.Add(NewComponent);
        }
    }
}

