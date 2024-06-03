#include "CameraFollowBehavior.h"
#include "CameraBehavior.h"

#include "../SubSystems/PlayerManager.h"
#include "../Actors/LuminariaCamera.h"
#include "../Actors/CameraArea.h"
#include "Components/SplineComponent.h"

void UCameraFollowBehavior::BeginBehavior(ALuminariaCamera* Owner) {
	Super::BeginBehavior(Owner);

	PlayerManager->OnPlayerRegister.AddDynamic(this, &UCameraFollowBehavior::OnRegisterPlayer);

}

void UCameraFollowBehavior::TickBehavior(float DeltaTime) {
	Super::TickBehavior(DeltaTime);

	if (!OwnerActor || !OwnerActor->CurrentArea || !OwnerActor->CurrentArea->Spline) {
		return;
	}

	if (!PlayerManager) {
		return;
	}

	if (PlayerManager->GetAllCharactersRef().Num() >= 2) {
		CalculateBarycenter();

		
		FVector SplinePosition = OwnerActor->CurrentArea->Spline->GetLocationAtDistanceAlongSpline(0, ESplineCoordinateSpace::World);

		// Get Projection Of Barycenter On Spline
		FVector BarycenterOnSpline = FVector(0, Barycenter.Y,0);

		FVector CamOnSpline = FVector(0, SplinePosition.Y,0);

		float Distance = FVector::Distance(BarycenterOnSpline, CamOnSpline);

		FVector NewLocation = OwnerActor->CurrentArea->Spline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);

		//GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Yellow, FString::Printf(TEXT("BarycenterOnSpline %s"),*BarycenterOnSpline.ToString()));
		//GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Green, FString::Printf(TEXT("Distance %f"),Distance));
		
		//GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Magenta, FString::Printf(TEXT("NewLoc %s"), *NewLocation.ToString()));

		OwnerActor->SetActorLocation(NewLocation);
	}
}

void UCameraFollowBehavior::OnRegisterPlayer(ADiamondProjectCharacter* Character) {
	_characters.Add(Character);

	if (PlayerManager->GetAllCharactersRef().Num() == 2) {
		DistanceToSpline = FVector::Distance(FVector(Character->GetActorLocation().X,0,0),FVector(OwnerActor->CurrentArea->Spline->GetComponentLocation().X,0, 0));
		GEngine->AddOnScreenDebugMessage(-1, 10.F, FColor::Green, FString::Printf(TEXT("Distance %f"), DistanceToSpline));
	}
}
