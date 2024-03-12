#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MecanismActivator.h"
#include "DefaultGenerator.h"
#include "DelayGenerator.generated.h"


UCLASS()
class DIAMONDPROJECT_API ADelayGenerator : public ADefaultGenerator {
	GENERATED_BODY()

	ADelayGenerator();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnMecanismActivateDelay(AMecanism* mecanism, AMecanismActivator* mecanismActivator);

	UFUNCTION()
	void OnMecanismDeactivateDelay(AMecanism* mecanism, AMecanismActivator* mecanismActivator);

private:

	UPROPERTY(EditAnywhere)
	float Delay;
};
