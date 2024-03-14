#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeathArea.generated.h"

UCLASS()
class DIAMONDPROJECT_API ADeathArea : public AActor
{
	GENERATED_BODY()
	
public:	
	ADeathArea();

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBoxComponent> BoxCollision;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void RegisterPlayer(ADiamondProjectCharacter* Character);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TArray<ADiamondProjectCharacter*> Characters;

};
