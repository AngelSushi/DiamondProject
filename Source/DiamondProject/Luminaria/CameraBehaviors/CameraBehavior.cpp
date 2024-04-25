
#include "CameraBehavior.h"
#include "DiamondProject/Luminaria/Actors/LuminariaCamera.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerEventsDispatcher.h"
#include "DiamondProject/Luminaria/Actors/CameraArea.h"

void UCameraBehavior::BeginBehavior(ALuminariaCamera* Owner) {
	PlayerEventsDispatcher = Owner->GetWorld()->GetSubsystem<UPlayerEventsDispatcher>();

	OwnerActor = Owner;

	_defaultY = OwnerActor->GetActorLocation().X;
}

void UCameraBehavior::TickBehavior(float DeltaTime) {

	FVector SphereCenter = OwnerActor->CurrentArea->GetActorLocation();
	SphereCenter.Y = OwnerActor->CurrentArea->MinPosition.X;

	if (OwnerActor->bDebugCamera) {
		DrawDebugSphere(GetWorld(), SphereCenter, 30.f, 8, FColor::Yellow, false, 1.F, 1, 3.F);
	}

	// WARNING : When both are true --> what happened ? 
	for (ADiamondProjectCharacter* Character : PlayerEventsDispatcher->Characters) {

		FVector Forward = Character->GetActorForwardVector();
		FVector BehindPlayerPos = Character->GetActorLocation() - Forward * Character->GetSimpleCollisionRadius() * 5.f;

		DrawDebugSphere(GetWorld(), BehindPlayerPos, 30.F, 8, FColor::Magenta, false, 1.F, 1, 3.F);

		// Check For Left Corner Of Area	
		
		if (FMath::IsNearlyEqual(Forward.X, 0.f) && FMath::IsNearlyEqual(Forward.Y, -1.f) && FMath::IsNearlyEqual(Forward.Z, 0.f)) { // If Player Goes To Left
			if (IsInFrustum(Character,SphereCenter)) { // Check if Left Border Of Area Is In Frustum 
				if (!bBlock) {
					CameraBlock();
				}

				bBlock = true;
			}
		}
		else if(FMath::IsNearlyEqual(Forward.X, 0.f) && FMath::IsNearlyEqual(Forward.Y, 1.f) && FMath::IsNearlyEqual(Forward.Z, 0.f)) { // If Player Goes To Right
			if (IsInFrustum(Character, SphereCenter) &&  bBlock) { // Check If Current Border Is Already In Frustrum and If Camera Is Block  
				if (BehindPlayerPos.Y >= OwnerActor->CurrentArea->MinPosition.X) {
					bBlock = false;
				}
			}
		}
	}
}

void UCameraBehavior::OnPlayerMove(ADiamondProjectCharacter* Character, FVector Direction, bool& IsCanceled) {
}

void UCameraBehavior::CameraBlock() {}

bool UCameraBehavior::IsInFrustum(ADiamondProjectCharacter* Character,FVector Position) {
	ULocalPlayer* LocalPlayer = Character->GetWorld()->GetFirstLocalPlayerFromController();

	if (LocalPlayer != nullptr && LocalPlayer->ViewportClient != nullptr && LocalPlayer->ViewportClient->Viewport) {
		FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(LocalPlayer->ViewportClient->Viewport, Character->GetWorld()->Scene, LocalPlayer->ViewportClient->EngineShowFlags).SetRealtimeUpdate(true));

		FVector ViewLocation;
		FRotator ViewRotation;
		FSceneView* SceneView = LocalPlayer->CalcSceneView(&ViewFamily, ViewLocation, ViewRotation, LocalPlayer->ViewportClient->Viewport);

		if (SceneView != nullptr) {
			if (SceneView->ViewFrustum.IntersectSphere(Position, 30.f)) {
				return true;
			}
			else {
				return false;
			}
		}
	}

	return false;
}

void UCameraBehavior::CalculateBarycenter() {
	FVector First = _characters[0]->GetActorLocation();
	FVector Second = _characters[1]->GetActorLocation();

	float divider = 2.F;
		
	_barycenter = (First + Second) / divider;

	_barycenter += FVector(0, 0, 45.F);
	_barycenter.X = _defaultY;
}

