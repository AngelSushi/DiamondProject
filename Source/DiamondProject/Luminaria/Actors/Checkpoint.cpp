#include "DiamondProject/Luminaria/Actors/Checkpoint.h"

ACheckpoint::ACheckpoint() {
	checkPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Checkpoint"));
	checkPoint->SetupAttachment(RootComponent);
}
