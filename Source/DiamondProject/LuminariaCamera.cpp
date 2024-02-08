#include "LuminariaCamera.h"

void ALuminariaCamera::BeginPlay()
{
	Super::BeginPlay();

	switch(CameraBehavior)
	{
		case DEFAULT:
		default:
			break;

		case LEADER:
			break;

		case SPLIT:
			break;
		case DYNAMIC:
			break;

		case DYNAMIC_SPLIT:
			break;
	}
}
