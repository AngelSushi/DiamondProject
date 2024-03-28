#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "MecanismComponent.generated.h"


UCLASS()
class DIAMONDPROJECT_API UMecanismComponent : public UStaticMeshComponent {
	GENERATED_BODY()

public:
	UMecanismComponent();
	
	UFUNCTION()
	void GenerateMesh(FVector Location);

private:

	UPROPERTY()
	FVector MeshLocation;
	
};
