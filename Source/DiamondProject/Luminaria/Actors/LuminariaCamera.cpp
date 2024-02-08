#include "LuminariaCamera.h"

#include "DiamondProject/Luminaria/ActorComponents/CameraLeaderBehavior.h"

void ALuminariaCamera::BeginPlay()
{
	Super::BeginPlay();

	switch(CameraBehavior)
	{
		case DEFAULT:
		default:
			break;

	case LEADER:
			AddComponentByClass(UCameraLeaderBehavior::StaticClass(),false,GetActorTransform(),false);
			break;

		case SPLIT:
			break;
		case DYNAMIC:
			break;

		case DYNAMIC_SPLIT:
			break;
	}
}
