#pragma once

#include "CoreMinimal.h"
#include "MecanismResultAction.h"
#include "MRA_PropsDissapear.generated.h"

UCLASS()
class DIAMONDPROJECT_API UMRA_PropsDissapear : public UMecanismResultAction {
	GENERATED_BODY()
	
public:
	virtual void OnMecanismTurnOn() override;
	
};
