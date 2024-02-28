
#include "CameraBehavior.h"
#include "DiamondProject/Luminaria/Actors/LuminariaCamera.h"

UCameraBehavior::UCameraBehavior()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCameraBehavior::BeginPlay()
{
	Super::BeginPlay();

	if(ALuminariaCamera* Camera = Cast<ALuminariaCamera>(GetOwner()))
	{
		OwnerActor = Camera;
	}
}


void UCameraBehavior::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// find out which way is forward
	const FRotator Rotation = OwnerActor->GetActorRotation(); // Prendre la caméra 
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
}

