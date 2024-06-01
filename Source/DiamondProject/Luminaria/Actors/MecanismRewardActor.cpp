#include "DiamondProject/Luminaria/Actors/MecanismRewardActor.h"
#include "Kismet/GameplayStatics.h"
#include "DiamondProject/Luminaria/Actors/Mecanism.h"


AMecanismRewardActor::AMecanismRewardActor(){

}

void AMecanismRewardActor::BeginPlay(){
	Super::BeginPlay();

	TArray<AActor*> MecanismArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMecanism::StaticClass(), MecanismArray);

	for (AActor* MecanismActor : MecanismArray) {
		if (AMecanism* Mecanism = Cast<AMecanism>(MecanismActor)) {
			if (Mecanism->MecanismResults.Contains(this)) {
				TargetMecanism = Mecanism;
				break;
			}
		}
	}
}

void AMecanismRewardActor::Reward() {
	GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Green, TEXT("Reward From CPP"));
	RewardFunction();
}

void AMecanismRewardActor::CancelReward() {
	CancelRewardFunction();
}



