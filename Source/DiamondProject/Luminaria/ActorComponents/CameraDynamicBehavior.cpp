#include "CameraDynamicBehavior.h"

#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"
#include "DiamondProject/Luminaria/Actors/LuminariaCamera.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerEventsDispatcher.h"


UCameraDynamicBehavior::UCameraDynamicBehavior() {}

void UCameraDynamicBehavior::BeginPlay()
{
	Super::BeginPlay();
	
	UPlayerEventsDispatcher* PlayerEventsDispatcher = GetWorld()->GetSubsystem<UPlayerEventsDispatcher>();
	PlayerEventsDispatcher->OnPlayerRegister.AddDynamic(this,&UCameraDynamicBehavior::OnRegisterPlayer);
	PlayerEventsDispatcher->OnPlayerMove.AddDynamic(this,&UCameraDynamicBehavior::OnPlayerMove);
	
	_defaultCameraPosition = OwnerActor->GetActorLocation();

	_maxZoomDistance = _minZoomDistance * 5.f;
}

void UCameraDynamicBehavior::OnRegisterPlayer(ADiamondProjectCharacter* player)
{
	_characters.Add(player);
	
	bool isCanceled = false;
	OnPlayerMove(player,FVector2D(),isCanceled);
}

void UCameraDynamicBehavior::OnPlayerMove(ADiamondProjectCharacter* player, FVector2D direction, bool& isCanceled)
{
	if(_characters.Num() >= 2)
	{
		FVector First = _characters[0]->GetActorLocation();
		FVector Second = _characters[1]->GetActorLocation();

		_barycenter = (First + Second) / 2.F;
		_barycenter += FVector(0,0,45.F);
	}
}

void UCameraDynamicBehavior::TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(_characters.Num() >= 2 && _barycenter != FVector::ZeroVector)
	{
		FVector smoothPosition = FMath::Lerp(OwnerActor->GetActorLocation(),_barycenter,50 * DeltaTime);
		
		float distance = FVector::Distance( _characters[0]->GetActorLocation(),_characters[1]->GetActorLocation());
		float zoomAlpha = FMath::Clamp((distance - _minZoomDistance) / (_maxZoomDistance - _minZoomDistance),0.F,1.F);
	
		float lerpY = FMath::Lerp(_defaultCameraPosition.Y,(_defaultCameraPosition + FVector(0,1,0) * 500).Y,zoomAlpha);
		smoothPosition.Y = lerpY;

		OwnerActor->SetActorLocation(smoothPosition);
	}
}


