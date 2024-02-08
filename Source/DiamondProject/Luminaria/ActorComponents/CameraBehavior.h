#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraBehavior.generated.h"


class ALuminariaCamera;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIAMONDPROJECT_API UCameraBehavior : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCameraBehavior();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<ALuminariaCamera> OwnerActor;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
