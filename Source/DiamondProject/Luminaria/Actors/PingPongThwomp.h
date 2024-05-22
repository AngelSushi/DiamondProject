// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/WidgetComponent.h"



#include "PingPongThwomp.generated.h"



UCLASS()
class DIAMONDPROJECT_API APingPongThwomp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APingPongThwomp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CubeMesh;

	UPROPERTY(EditAnywhere)
	UWidgetComponent* StartWidgetComponent; // Composant de widget pour le point de départ

	UPROPERTY(EditAnywhere)
	UWidgetComponent* EndWidgetComponent; 
	
	UPROPERTY(VisibleAnywhere)
	FVector StartPoint;

	UPROPERTY(VisibleAnywhere)
	FVector EndPoint;

	UPROPERTY(EditAnywhere)
	float Speed;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UDeathComponent> DeathComponent;

	FVector CurrentTarget;

};
