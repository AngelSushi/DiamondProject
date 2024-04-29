#include "CameraLeaderBehavior.h"

#include "DiamondProject/Luminaria/Actors/LuminariaCamera.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerManager.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"

void UCameraLeaderBehavior::BeginBehavior(ALuminariaCamera* Owner) {
	Super::BeginBehavior(Owner);
	GEngine->AddOnScreenDebugMessage(-1,15.F,FColor::Yellow,TEXT("Add Leader Behavior"));
	
	PlayerManager->OnPlayerRegister.AddDynamic(this,&UCameraLeaderBehavior::RegisterPlayer);
	PlayerManager->OnPlayerDeath.AddDynamic(this, &UCameraLeaderBehavior::OnPlayerDeath);
}

void UCameraLeaderBehavior::RegisterPlayer(ADiamondProjectCharacter* Character) {
	_characters.AddUnique(Character);
	GEngine->AddOnScreenDebugMessage(-1,15.F,FColor::Yellow,TEXT("Register Player From Leader Behavior"));
	_leader = _characters[FMath::RandRange(0,_characters.Num() - 1)];
}

void UCameraLeaderBehavior::TickBehavior(float DeltaTime) {
	Super::TickBehavior(DeltaTime);

	if (_leader != nullptr) {
		FVector LeaderPosition = _leader->GetActorLocation();
		FVector CamPosition = OwnerActor->GetActorLocation();

		FVector NewPosition = FVector(0, 0, LeaderPosition.Z + 45.F);

		NewPosition.X = CamPosition.X;
		NewPosition.Y = LeaderPosition.Y;


		OwnerActor->SetActorLocation(NewPosition);
	}
}

void UCameraLeaderBehavior::OnPlayerDeath(ADiamondProjectCharacter* deathPlayer) {
	_leader = *_characters.FindByPredicate([&deathPlayer](const ADiamondProjectCharacter* player) {
		return deathPlayer != player;
	});
}







