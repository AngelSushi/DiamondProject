#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MecanismRewardActor.h"
#include "MovingPlatefomCPPTest.generated.h"

UCLASS()
class DIAMONDPROJECT_API AMovingPlatefomCPPTest : public AMecanismRewardActor {
	GENERATED_BODY()
	
public:	
	AMovingPlatefomCPPTest();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform Settings")
    TArray<AActor*> Waypoints;

    UPROPERTY(EditAnywhere)
    TObjectPtr<class UStaticMeshComponent> Cube;

    UPROPERTY(EditAnywhere)
    TObjectPtr<class UBoxComponent> BoxBlock;

    UPROPERTY(EditAnywhere)
    TObjectPtr<class UBoxComponent> BoxOverlap;
  

    UFUNCTION(BlueprintPure)
    UPlateformDataAsset* GetPlateformAsset() { return PlateformAsset; }

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

private:
    UPROPERTY(EditAnywhere, Category = "Platform Settings")
    TObjectPtr<class UPlateformDataAsset> PlateformAsset;

    FVector StartLocation;
    int32 CurrentWaypointIndex;
    bool bMovingForward;

    UPROPERTY()
    bool bEnable;

    void OnMecanismOn(AMecanism* Mecanism);
    void OnMecanismOff(AMecanism* Mecanism);

    UPROPERTY(EditAnywhere)
    TObjectPtr<class UMaterial> CrystalMaterialRef;

    UFUNCTION()
    void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UPROPERTY()
    float PlayerOn = 0;

    UPROPERTY()
    TArray<UMaterialInstanceDynamic*> CrystalsMat;

};
