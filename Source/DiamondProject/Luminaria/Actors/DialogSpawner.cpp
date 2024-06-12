#include "DiamondProject/Luminaria/Actors/DialogSpawner.h"
#include "Components/BillboardComponent.h"
#include "Components/ArrowComponent.h"

#include "DiamondProject/Luminaria/SubSystems/PlayerManager.h"

ADialogSpawner::ADialogSpawner() {
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(RootComponent);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);
}

//void ADialogSpawner::OnDisplayDialog(int DialogIndex) {}

void ADialogSpawner::BeginPlay() {
	Super::BeginPlay();

	PlayerManager = GetWorld()->GetSubsystem<UPlayerManager>();
}

void ADialogSpawner::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	int Index = 0;

	for (ADiamondProjectCharacter* Character : PlayerManager->GetAllCharactersRef()) {
		FVector PointToPlayer = GetActorLocation() - Character->GetActorLocation();
		FVector Forward = GetActorForwardVector();

		float Angle = FVector::DotProduct(PointToPlayer, Forward);
		float DistanceY = FMath::Abs(PointToPlayer.Y);

		if (Angle < 0) {
			Index++;
		}
	}

	if (Index == 2) {
		OnDisplayDialog();
		bHasChecked = true;
	}
	else if(Index == 0 && bHasChecked) {
		OnLeaveDialog();
		bHasChecked = false;
	}
}

