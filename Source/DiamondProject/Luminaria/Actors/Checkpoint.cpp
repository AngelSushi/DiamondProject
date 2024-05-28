#include "DiamondProject/Luminaria/Actors/Checkpoint.h"
#include "Components/BillboardComponent.h"

ACheckpoint::ACheckpoint() {
	checkPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Checkpoint"));
	checkPoint->SetupAttachment(RootComponent);

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(checkPoint);
}
