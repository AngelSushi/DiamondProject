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

	virtual void OnConstruction(const FTransform& Transform) override;

public:	

	// Variable exposée dans l'éditeur
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	int32 NumberOfComponents;

	// Fonction pour obtenir les positions des composants
	UFUNCTION(BlueprintCallable, Category = "Components")
	TArray<FVector> GetComponentPositions() const;
private:
	

	// Composant parent pour contenir les meshes ajoutés manuellement
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* MeshFolder;

	// Composant parent pour contenir les composants dynamiques
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* SceneFolder;

	// Liste pour stocker les références des composants ajoutés
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TArray<USceneComponent*> SceneComponents;

	// Fonction pour ajouter des composants
	void AddComponents();
};



