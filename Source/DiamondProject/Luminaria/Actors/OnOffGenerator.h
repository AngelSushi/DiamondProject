#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MecanismActivator.h"
#include "DefaultGenerator.h"
#include "OnOffGenerator.generated.h"

class ADefaultGenerator;

UCLASS()
class DIAMONDPROJECT_API AOnOffGenerator : public ADefaultGenerator {
	GENERATED_BODY()

	AOnOffGenerator();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnMecanismDeactivateOnOff(AMecanism* mecanism, AMecanismActivator* mecanismActivator);


public:
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


private:
	UPROPERTY()
	TObjectPtr<class UMaterial> basicMaterial;
	
};
