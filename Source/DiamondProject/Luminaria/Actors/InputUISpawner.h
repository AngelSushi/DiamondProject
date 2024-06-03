#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InputUISpawner.generated.h"

UCLASS()
class DIAMONDPROJECT_API AInputUISpawner : public AActor {
	GENERATED_BODY()
	
public:	
	AInputUISpawner();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
