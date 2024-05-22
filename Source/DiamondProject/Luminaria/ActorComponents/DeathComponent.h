#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "DeathComponent.generated.h"

class UPlayerManager;
enum EDeathCause;

UCLASS()
class DIAMONDPROJECT_API UDeathComponent : public UBoxComponent {
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void RegisterPlayer(ADiamondProjectCharacter* Character);
	
private:
	UPROPERTY()
	TArray<ADiamondProjectCharacter*> Characters;

	UPROPERTY()
	UPlayerManager* PlayerManager;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EDeathCause> DeathCause;
};
