#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrowthOrb.generated.h"

UCLASS()
class DIAMONDPROJECT_API AGrowthOrb : public AActor {
	GENERATED_BODY()
	
public:	
	AGrowthOrb();

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<class UStaticMeshComponent> MeshComponent;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
