#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Link.generated.h"

UCLASS()
class DIAMONDPROJECT_API ALink : public AActor{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> _mesh;

	UPROPERTY()
	TArray<ADiamondProjectCharacter*> _characters;

	UFUNCTION()
	void RegisterPlayer(ADiamondProjectCharacter* character);

	UFUNCTION()
	void CalculateBarycenter();

	UPROPERTY()
	FVector _barycenter;
public:	
	ALink();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
