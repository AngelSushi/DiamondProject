#include "CameraDynamicBehavior.h"

#include "Camera/CameraComponent.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"
#include "Luminaria/Actors/LuminariaCamera.h"
#include "Luminaria/SubSystems/PlayerEventsDispatcher.h"


UCameraDynamicBehavior::UCameraDynamicBehavior() {}

void UCameraDynamicBehavior::BeginPlay()
{
	Super::BeginPlay();
	
	UPlayerEventsDispatcher* PlayerEventsDispatcher = GetWorld()->GetSubsystem<UPlayerEventsDispatcher>();
	PlayerEventsDispatcher->OnPlayerRegister.AddDynamic(this,&UCameraDynamicBehavior::OnRegisterPlayer);
	PlayerEventsDispatcher->OnPlayerMove.AddDynamic(this,&UCameraDynamicBehavior::OnPlayerMove);
	
	_defaultCameraPosition = OwnerActor->GetActorLocation();
}

void UCameraDynamicBehavior::OnRegisterPlayer(ACharacter* player)
{
	_characters.Add(player);
	
	bool isCanceled = false;
	OnPlayerMove(player,FVector2D(),isCanceled);
}

void UCameraDynamicBehavior::OnPlayerMove(ACharacter* player, FVector2D direction, bool& isCanceled)
{
	if(_characters.Num() >= 2)
	{
		FVector First = _characters[0]->GetActorLocation();
		FVector Second = _characters[1]->GetActorLocation();

		_barycenter = (First + Second) / 2.F;
		_barycenter += FVector(0,0,45.F);
		_barycenter.Y = _defaultCameraPosition.Y;
	}
}

void UCameraDynamicBehavior::TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(_characters.Num() >= 2)
	{
		CalculateZoomDistance(_minZoomDistance,_maxZoomDistance);
		
		float distance = FVector::Distance( _characters[0]->GetActorLocation(),_characters[1]->GetActorLocation());
		float zoomAlpha = FMath::Clamp((distance - _minZoomDistance) / (_maxZoomDistance - _minZoomDistance),0.F,1.F);
		
		FVector smoothPosition = FMath::Lerp(OwnerActor->GetActorLocation(),_barycenter,50 * DeltaTime);
		float lerpY = FMath::Lerp(_defaultCameraPosition.Y,(_defaultCameraPosition + FVector(0,1,0) * 500).Y,zoomAlpha * 1.1f);

		smoothPosition.Y = lerpY;

		OwnerActor->SetActorLocation(smoothPosition);
	}
}

void UCameraDynamicBehavior::CalculateZoomDistance(float& minDistance, float& maxDistance)
{
	if(_minZoomDistance == 0.F)
	{
		if(APlayerController* PlayerController = Cast<APlayerController>( _characters[0]->GetController()))
		{
			FVector RightViewportCorner = FVector();
			FVector RightViewportCornerDirection = FVector();

			FVector2D ViewportSize = FVector2D();
			GEngine->GameViewport->GetViewportSize(ViewportSize);

			PlayerController->DeprojectScreenPositionToWorld(ViewportSize.X,0,RightViewportCorner,RightViewportCornerDirection);

			minDistance = RightViewportCorner.Y - 100.F;
			maxDistance = _minZoomDistance * 1.5f;
		}
	}
}
