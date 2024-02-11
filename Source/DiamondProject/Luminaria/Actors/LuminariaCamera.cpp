#include "LuminariaCamera.h"

#include "DiamondProject/CameraDynamicBehavior.h"
#include "DiamondProject/Luminaria/ActorComponents/CameraDefaultBehavior.h"
#include "DiamondProject/Luminaria/ActorComponents/CameraLeaderBehavior.h"

void ALuminariaCamera::BeginPlay()
{
	Super::BeginPlay();

	switch(CameraBehavior)
	{
		case DEFAULT:
		default:
			AddComponentByClass(UCameraDefaultBehavior::StaticClass(),false,GetActorTransform(),false);
			break;

		case LEADER:
			AddComponentByClass(UCameraLeaderBehavior::StaticClass(),false,GetActorTransform(),false);
			break;
		case DYNAMIC:
			AddComponentByClass(UCameraDynamicBehavior::StaticClass(),false,GetActorTransform(),false);
			break;
	}
}
