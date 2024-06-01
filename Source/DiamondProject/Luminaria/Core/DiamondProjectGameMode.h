#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DiamondProjectGameMode.generated.h"

class ADiamondProjectCharacter;

UCLASS(minimalapi)
class ADiamondProjectGameMode : public AGameModeBase {
	GENERATED_BODY()

public:
	ADiamondProjectGameMode();

	UPROPERTY(EditAnywhere)
	TSubclassOf<ADiamondProjectCharacter> Character;


protected:
	virtual void BeginPlay() override;

private:
	void CreatePlayer(int ControllerId, AActor* PlayerStart);
};



