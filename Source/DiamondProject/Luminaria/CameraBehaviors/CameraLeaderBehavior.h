#pragma once

#include "CoreMinimal.h"
#include "CameraBehavior.h"
#include "CameraLeaderBehavior.generated.h"

UCLASS()
class DIAMONDPROJECT_API UCameraLeaderBehavior : public UCameraBehavior {
	GENERATED_BODY()
	
public:
	virtual void BeginBehavior(ALuminariaCamera* Owner) override;

	virtual void TickBehavior(float DeltaTime) override;
private:
	UPROPERTY()
	ADiamondProjectCharacter* _leader;
	
	UFUNCTION()
	void RegisterPlayer(ADiamondProjectCharacter* Character);
	
	UFUNCTION()
	void OnPlayerDeath(ADiamondProjectCharacter* deathPlayer,EDeathCause DeathCause);
};
