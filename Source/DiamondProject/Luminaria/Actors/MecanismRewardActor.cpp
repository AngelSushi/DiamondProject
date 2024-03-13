#include "DiamondProject/Luminaria/Actors/MecanismRewardActor.h"
#include "Kismet/GameplayStatics.h"
#include "DiamondProject/Luminaria/Actors/Mecanism.h"


AMecanismRewardActor::AMecanismRewardActor(){

}

void AMecanismRewardActor::BeginPlay(){
	Super::BeginPlay();
}

void AMecanismRewardActor::Reward() {
	GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Green, TEXT("Reward From CPP"));
	RewardFunction();
}

void AMecanismRewardActor::CancelReward() {
	CancelRewardFunction();
}



