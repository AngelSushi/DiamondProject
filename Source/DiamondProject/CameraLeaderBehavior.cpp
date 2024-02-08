#include "CameraLeaderBehavior.h"

UCameraLeaderBehavior::UCameraLeaderBehavior()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCameraLeaderBehavior::BeginPlay()
{
	Super::BeginPlay();

	
}

void UCameraLeaderBehavior::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(Leader != nullptr)
	{
		
	}
}


