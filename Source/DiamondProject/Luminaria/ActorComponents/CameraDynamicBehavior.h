#pragma once

#include "CoreMinimal.h"
#include "DiamondProject/Luminaria/ActorComponents/CameraBehavior.h"
#include "CameraDynamicBehavior.generated.h"


USTRUCT()
struct FExtendData {

	GENERATED_BODY()

public:
	FVector position;
	FVector direction;
	FVector cameraPosition;
};

UCLASS()
class DIAMONDPROJECT_API UCameraDynamicBehavior : public UCameraBehavior
{
	GENERATED_BODY()

	UCameraDynamicBehavior();

public:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRegisterPlayer(ADiamondProjectCharacter* player);

	UFUNCTION()
	void OnPlayerMove(ADiamondProjectCharacter* character, FVector direction, bool& isCanceled);
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	FVector _offset;

	UPROPERTY()
	bool _canExtend;

	UPROPERTY()
	TMap<FVector, FVector> _extendPositions;
};
