#include "CameraDynamicBehavior.h"

#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"
#include "DiamondProject/Luminaria/Actors/LuminariaCamera.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerManager.h"
#include "DiamondProject/Luminaria/Actors/CameraArea.h"
#include "DiamondProject/Luminaria/CameraBehaviors/HeightCameraBehavior.h"

void UCameraDynamicBehavior::BeginBehavior(ALuminariaCamera* Owner) {
	Super::BeginBehavior(Owner);
	
	PlayerManager->OnPlayerMove.AddDynamic(this, &UCameraDynamicBehavior::OnPlayerMove);

	OffsetX = DefaultX;
	Barycenter.X = OffsetX;
}


void UCameraDynamicBehavior::OnPlayerMove(ADiamondProjectCharacter* character, FVector direction, bool& isCanceled) {
	Super::OnPlayerMove(character, direction, isCanceled);

	CalculateOffsideFrustumOffset(character, direction);

	if (_extendPositions.Num() > 0) {
		_extendPositions.RemoveAll([this, &character, &direction](const FExtendData& extendData) {
			FVector ExtendToPlayer = extendData.Position - character->GetActorLocation();
			FVector Forward = extendData.Direction;

			float angle = FVector::DotProduct(ExtendToPlayer, Forward);
			float Distance = FVector::Distance(extendData.Position, character->GetActorLocation());

			if (angle < 0 && Forward == -direction && Distance < 100.F) {
				OffsetX += FMath::Abs(extendData.Offset);
				return true;
			}
			
			return false;
		});
	}
	
}


void UCameraDynamicBehavior::TickBehavior(float DeltaTime) {
	Super::TickBehavior(DeltaTime);

	if(PlayerManager->Characters.Num() >= 2) {

		if (!bBlock) {
			float ToApproachY = (PlayerManager->Characters[0]->GetActorLocation().Y + PlayerManager->Characters[1]->GetActorLocation().Y) / 2;

			if (Barycenter.Y == 0.F) {
				Barycenter.Y = ToApproachY;
			}

			Barycenter.Y = Approach(Barycenter.Y, ToApproachY,350 * DeltaTime);
			Barycenter.Z = DefaultZ;
			Barycenter.X = Approach(Barycenter.X, OffsetX, 350 * DeltaTime);

			if (OwnerActor->CurrentArea) {
				Barycenter.X = FMath::Clamp(Barycenter.X, OwnerActor->CurrentArea->ZoomMin, OwnerActor->CurrentArea->ZoomMax);
			}
		}

		OwnerActor->SetActorLocation(Barycenter);

		if (OwnerActor->bDebugCamera) {
			for (auto& extendPosition : _extendPositions) {
				DrawDebugLine(OwnerActor->GetWorld(), extendPosition.Position, extendPosition.Position + FVector::UpVector * 250.f, FColor::White, false, 1.F, 1, 2.F);
				DrawDebugLine(OwnerActor->GetWorld(), extendPosition.Position, extendPosition.Position + extendPosition.Direction * 150.f, FColor::Green, false, 1.F, 1, 2.F);
			}
		}
	}
}

void UCameraDynamicBehavior::CalculateOffsideFrustumOffset(ADiamondProjectCharacter* character,FVector direction) {
	ULocalPlayer* LocalPlayer = character->GetWorld()->GetFirstLocalPlayerFromController();

	if (bBlock) {
		return;
	}

	if (LocalPlayer != nullptr && LocalPlayer->ViewportClient != nullptr && LocalPlayer->ViewportClient->Viewport) {
		FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(LocalPlayer->ViewportClient->Viewport, character->GetWorld()->Scene, LocalPlayer->ViewportClient->EngineShowFlags).SetRealtimeUpdate(true));

		FVector ViewLocation;
		FRotator ViewRotation;
		FSceneView* SceneView = LocalPlayer->CalcSceneView(&ViewFamily, ViewLocation, ViewRotation, LocalPlayer->ViewportClient->Viewport);

		if (SceneView != nullptr) {
			FVector Center = character->GetActorLocation() + direction * 200.F;
			FVector PlayerCenter = character->GetActorLocation();

			if (OwnerActor->bDebugCamera) {
				DrawDebugSphere(OwnerActor->GetWorld(), Center, character->GetSimpleCollisionRadius(), 8, FColor::Green, false, 1.F, 1, 3.F);
				DrawDebugSphere(OwnerActor->GetWorld(), PlayerCenter, character->GetSimpleCollisionRadius(), 8, FColor::Red, false, 1.F, 1, 3.F);
			}

			if (!SceneView->ViewFrustum.IntersectSphere(Center, character->GetSimpleCollisionRadius())   && SceneView->ViewFrustum.IntersectSphere(PlayerCenter, character->GetSimpleCollisionRadius())) {

				Center.Y = character->GetActorLocation().Y;

				for (auto& extendPosition : _extendPositions) {
					if (FVector::Distance(extendPosition.Position,Center) < 250.F) {
						return;
					}
				}

				float DistanceToLeftBorder = FMath::Abs(character->GetActorLocation().Y - OwnerActor->CurrentArea->MinPosition.X);
				float DistanceToRightBorder = FMath::Abs(OwnerActor->CurrentArea->MaxPosition.X - character->GetActorLocation().Y);
				float Offset = 0.F;

				if (DistanceToLeftBorder <= 250.F) {
					Offset = DistanceToLeftBorder;
				}
				else if (DistanceToRightBorder <= 250.F) {
					Offset = DistanceToRightBorder;
				}
				else {
					Offset = 250.F;
				}

				OffsetX -= Offset;

				_extendPositions.Add(FExtendData(Center, direction, _extendPositions.Num(),Offset));
			}
		}
	}
}




