
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

	_defaultY = OwnerActor->GetActorLocation().Y;
}


void UCameraBehavior::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// find out which way is forward
	const FRotator Rotation = OwnerActor->GetActorRotation(); // Prendre la cam�ra 
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
}

void UCameraBehavior::CalculateBarycenter() {
	FVector First = _characters[0]->GetActorLocation();
	FVector Second = _characters[1]->GetActorLocation();

	float divider = 2.F;

	if (!_characters[0]->GetMesh()->IsVisible()) {
		First = FVector::Zero();
		divider -= 1.F;
	}

	if (!_characters[1]->GetMesh()->IsVisible()) {
		Second = FVector::Zero();
		divider -= 1.F;
	}

	_barycenter = (First + Second) / divider;
	_barycenter += FVector(0, 0, 45.F);

	if (ForwardDirection.X != 0) {
		_barycenter.X = _defaultY;
	}
	else if (ForwardDirection.Y != 0) {
		_barycenter.Y = _defaultY;
	}

}

