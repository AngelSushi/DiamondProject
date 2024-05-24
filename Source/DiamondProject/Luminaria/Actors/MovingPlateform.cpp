// Fill out your copyright notice in the Description page of Project Settings.


#include "DiamondProject/Luminaria/Actors/MovingPlateform.h"
#include "Components/SceneComponent.h"

// Sets default values
AMovingPlateform::AMovingPlateform()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    NumberOfComponents = 0;

    //////// Créer et attacher le MeshFolder
    MeshFolder = CreateDefaultSubobject<USceneComponent>(TEXT("MeshFolder"));
    MeshFolder->SetupAttachment(RootComponent);

    //////// Créer et attacher le SceneFolder comme enfant de MeshFolder
    SceneFolder = CreateDefaultSubobject<USceneComponent>(TEXT("SceneFolder"));
    SceneFolder->SetupAttachment(MeshFolder);
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
        USceneComponent* NewComponent = NewObject<USceneComponent>(this, USceneComponent::StaticClass(), NAME_None, RF_Transactional);
        if (NewComponent)
        {
            NewComponent->AttachToComponent(SceneFolder, FAttachmentTransformRules::KeepRelativeTransform);
            NewComponent->RegisterComponent();
            SceneComponents.Add(NewComponent);
        }
    }
}

TArray<FVector> AMovingPlateform::GetComponentPositions() const
{
    TArray<FVector> Positions;

    for (USceneComponent* Component : SceneComponents)
    {
        // Vérifier que le parent du composant est bien SceneFolder
        if (Component && Component->GetAttachParent() == SceneFolder)
        {
            Positions.Add(Component->GetComponentLocation());
        }
    }

    return Positions;
}

