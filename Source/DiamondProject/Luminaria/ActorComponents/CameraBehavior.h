#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraBehavior.generated.h"


class ALuminariaCamera;
class ADiamondProjectCharacter;

UCLASS( ClassGroup=(Custom),Blueprintable, meta=(BlueprintSpawnableComponent) )
class DIAMONDPROJECT_API UCameraBehavior : public USceneComponent
{
	GENERATED_BODY()

public:	
	UCameraBehavior();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<ALuminariaCamera> OwnerActor;

	UPROPERTY()
	FVector ForwardDirection;

	UFUNCTION()
	void CalculateBarycenter();

	UPROPERTY()
	TArray<ADiamondProjectCharacter*> _characters;

	UPROPERTY()
	FVector _barycenter;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY()
	float _defaultY;
};
