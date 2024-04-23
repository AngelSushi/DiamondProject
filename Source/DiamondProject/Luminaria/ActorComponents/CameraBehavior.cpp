
#include "CameraBehavior.h"
#include "DiamondProject/Luminaria/Actors/LuminariaCamera.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"

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

	_defaultY = OwnerActor->GetActorLocation().X;
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

void UCameraBehavior::CalculateBarycenter() {
	FVector First = _characters[0]->GetActorLocation();
	FVector Second = _characters[1]->GetActorLocation();

	float divider = 2.F;
		
	_barycenter = (First + Second) / divider;

	_barycenter += FVector(0, 0, 45.F);
	_barycenter.X = _defaultY;
}

