#pragma once

#include "CoreMinimal.h"
#include "../Interface/ButtonInteractable.h"
#include "GameFramework/Actor.h"
#include "FallingPlateform.generated.h"

UCLASS()
class DIAMONDPROJECT_API AFallingPlateform : public AActor,public IButtonInteractable {
	GENERATED_BODY()
	
public:	
	AFallingPlateform();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


    UPROPERTY(EditAnywhere)
    TObjectPtr<class UStaticMeshComponent> PlatformMesh;

    UPROPERTY(EditAnywhere)
    TObjectPtr<class UBoxComponent> DetectionBox;

    UPROPERTY(EditAnywhere)
    float TimeBeforeFall;


    UPROPERTY(EditAnywhere)
    float ResetDelay;


private:
    UPROPERTY(VisibleAnywhere)
    bool bCharacterOnPlatform;

    UPROPERTY(VisibleAnywhere)
    bool bPlateformAlreadyFall;

    UPROPERTY(EditAnywhere)
    int32 numPlayersInside;

    UPROPERTY(VisibleAnywhere)
    bool bPlateformFall;

    UPROPERTY(VisibleAnywhere)
    float TimeSinceCharacterOnPlatform;

    UPROPERTY(VisibleAnywhere)
    float TimePlateformFall;


    FVector InitialLocation;


    void ShakeBox();

    bool bIsShaking;

    bool bIsMeshCollisionReset;

    UPROPERTY(VisibleAnywhere)
    float ShakeTime;

    UPROPERTY(EditAnywhere)
    float ShakeDuration;


    UFUNCTION()
    void OnCharacterOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
        const FHitResult& SweepResult);

    UFUNCTION()
    void OnCharacterOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    void FallPlatform();
    void ResetPlatform();
    void ResetMeshCollider();
};
