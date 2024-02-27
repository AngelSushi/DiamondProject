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
	
	_defaultCameraPosition = OwnerActor->GetActorLocation();

	_maxZoomDistance = _minZoomDistance * 5.f;
}

void UCameraDynamicBehavior::OnRegisterPlayer(ADiamondProjectCharacter* player)
{
	_characters.Add(player);
}


void UCameraDynamicBehavior::TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(_characters.Num() >= 2) {
		CalculateBarycenter();

		if (_barycenter == FVector::Zero())
			return;

		FVector smoothPosition = FMath::Lerp(OwnerActor->GetActorLocation(),_barycenter,50 * DeltaTime);
		
		float distance = FVector::Distance( _characters[0]->GetActorLocation(),_characters[1]->GetActorLocation());
		float zoomAlpha = FMath::Clamp((distance - _minZoomDistance) / (_maxZoomDistance - _minZoomDistance),0.F,1.F);
	
		float lerpY = FMath::Lerp(_defaultCameraPosition.Y,(_defaultCameraPosition + FVector(0,1,0) * 500).Y,zoomAlpha);
		smoothPosition.X = lerpY;

		GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Yellow, FString::Printf(TEXT("lerp %f"), lerpY));

		OwnerActor->SetActorLocation(smoothPosition);
	}
}

void UCameraDynamicBehavior::CalculateBarycenter() {
	FVector First = _characters[0]->GetActorLocation();
	FVector Second = _characters[1]->GetActorLocation();

	float divider = 2.F;

	if (!_characters[0]->GetMesh()->IsVisible()) {
		First = FVector::Zero();
		divider -= 1.F;
	}

	if (!_characters[1]->GetMesh()->IsVisible()) {
		Second = FVector::Zero();
		divider -= 1.F;
	}

	_barycenter = (First + Second) / divider;
	_barycenter += FVector(0, 0, 45.F);
}


