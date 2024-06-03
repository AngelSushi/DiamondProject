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

	for (ADiamondProjectCharacter* Character : PlayerManager->GetAllCharactersRef()) {
		FVector PointToPlayer = GetActorLocation() - Character->GetActorLocation();
		FVector Forward = GetActorForwardVector();

		float Angle = FVector::DotProduct(PointToPlayer, Forward);
		float DistanceX = FMath::Abs(PointToPlayer.X);
		
		if (Angle < 0 && DistanceX < 50.F && !bHasChecked) {
			//GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Black, TEXT("Display Text"));
			OnDisplayDialog();
			bHasChecked = true;	
		}	
	}
}

