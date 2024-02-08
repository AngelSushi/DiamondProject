#include "CameraLeaderBehavior.h"

#include "DiamondProject/Luminaria/Actors/LuminariaCamera.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerEventsDispatcher.h"
#include "GameFramework/Character.h"

UCameraLeaderBehavior::UCameraLeaderBehavior()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCameraLeaderBehavior::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1,15.F,FColor::Yellow,TEXT("Add Leader Behavior"));
	
	UPlayerEventsDispatcher* PlayerEventsDispatcher = GetWorld()->GetSubsystem<UPlayerEventsDispatcher>();
	PlayerEventsDispatcher->OnPlayerRegister.AddDynamic(this,&UCameraLeaderBehavior::RegisterPlayer);
}

void UCameraLeaderBehavior::RegisterPlayer(ACharacter* Character) 
{
	Characters.AddUnique(Character);
	GEngine->AddOnScreenDebugMessage(-1,15.F,FColor::Yellow,TEXT("Register Player From Leader Behavior"));
	Leader = Characters[FMath::RandRange(0,Characters.Num() - 1)];
}

void UCameraLeaderBehavior::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(Leader != nullptr)
	{
		FVector LeaderPosition = Leader->GetActorLocation();
		FVector CamPosition = OwnerActor->GetActorLocation();

		/// A CHANGER SELON LE FORWARD DU JOUEUR 

		FVector NewPosition = FVector(LeaderPosition.X,CamPosition.Y,LeaderPosition.Z + 45.F);

		OwnerActor->SetActorLocation(NewPosition);
	}
}




