#include "CameraLeaderBehavior.h"

#include "DiamondProject/Luminaria/Actors/LuminariaCamera.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerEventsDispatcher.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"

UCameraLeaderBehavior::UCameraLeaderBehavior()
{}

void UCameraLeaderBehavior::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1,15.F,FColor::Yellow,TEXT("Add Leader Behavior"));
	
	UPlayerEventsDispatcher* PlayerEventsDispatcher = GetWorld()->GetSubsystem<UPlayerEventsDispatcher>();
	PlayerEventsDispatcher->OnPlayerRegister.AddDynamic(this,&UCameraLeaderBehavior::RegisterPlayer);
	PlayerEventsDispatcher->OnPlayerDeath.AddDynamic(this, &UCameraLeaderBehavior::OnPlayerDeath);
}

void UCameraLeaderBehavior::RegisterPlayer(ADiamondProjectCharacter* Character) 
{
	_characters.AddUnique(Character);
	GEngine->AddOnScreenDebugMessage(-1,15.F,FColor::Yellow,TEXT("Register Player From Leader Behavior"));
	_leader = _characters[FMath::RandRange(0,_characters.Num() - 1)];
}

void UCameraLeaderBehavior::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (_leader != nullptr) {
		FVector LeaderPosition = _leader->GetActorLocation();
		FVector CamPosition = OwnerActor->GetActorLocation();

		FVector NewPosition = FVector(0, 0, LeaderPosition.Z + 45.F);

		if (ForwardDirection.X != 0) {
			NewPosition.X = CamPosition.X;
			NewPosition.Y = LeaderPosition.Y;
		}
		else if (ForwardDirection.Y != 0) {
			NewPosition.X = LeaderPosition.X;
			NewPosition.Y = CamPosition.Y;
		}

		OwnerActor->SetActorLocation(NewPosition);
	}
}

void UCameraLeaderBehavior::OnPlayerDeath(ADiamondProjectCharacter* deathPlayer) {
	_leader = *_characters.FindByPredicate([&deathPlayer](const ADiamondProjectCharacter* player) {
		return deathPlayer != player;
	});
}







