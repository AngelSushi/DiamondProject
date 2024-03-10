#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "MecanismComponent.generated.h"


UCLASS()
class DIAMONDPROJECT_API UMecanismComponent : public UStaticMeshComponent {
	GENERATED_BODY()

public:
	UMecanismComponent();

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UProceduralMeshComponent> proceduralMesh;
	
	UFUNCTION()
	void GenerateMesh(FVector Location);

private:
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UPROPERTY()
	FVector MeshLocation;
	
};
