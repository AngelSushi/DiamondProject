
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
}

