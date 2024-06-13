#include "CameraDynamicBehavior.h"

#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"
#include "DiamondProject/Luminaria/Actors/LuminariaCamera.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerManager.h"
#include "DiamondProject/Luminaria/Actors/CameraArea.h"
#include "DiamondProject/Luminaria/CameraBehaviors/HeightCameraBehavior.h"

#include "../DataAssets/CameraAreaDataAsset.h"
void UCameraDynamicBehavior::BeginBehavior(ALuminariaCamera* Owner) {
	Super::BeginBehavior(Owner);
	
	PlayerManager->OnPlayerMove.AddDynamic(this, &UCameraDynamicBehavior::OnPlayerMove);

	OffsetX = DefaultX; // Possiblement Faux passer a Location.X
	Barycenter.X = OwnerActor->GetActorLocation().X;
	Barycenter.Y = OwnerActor->GetActorLocation().Y;
	Barycenter.Z = OwnerActor->GetActorLocation().Z;

}


void UCameraDynamicBehavior::OnPlayerMove(ADiamondProjectCharacter* character,FVector2D Input,FVector direction,bool& isCanceled) { // VERIFIER QUE CELA SAPPELLE BIEN
	Super::OnPlayerMove(character,Input,direction, isCanceled);

	isCanceled	= false;

	CalculateOffsideFrustumOffset(character, direction);

	if (_extendPositions.Num() > 0) {
		_extendPositions.RemoveAll([this, &character, &direction](const FExtendData& extendData) {
			FVector ExtendToPlayer = extendData.Position - character->GetActorLocation();
			FVector Forward = extendData.Direction;

			float angle = FVector::DotProduct(ExtendToPlayer, Forward);

			//float Distance = FVector::Distance(extendData.Position, character->GetActorLocation());
			float DistanceX = FMath::Abs(extendData.Position.X - character->GetActorLocation().X);

			if (angle < 0 && Forward == -direction && DistanceX < 100.F) {
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
		
		float ToApproachY = (PlayerManager->Characters[0]->GetActorLocation().Y + PlayerManager->Characters[1]->GetActorLocation().Y) / 2;
			
		if (Barycenter.Y == 0.F) {
			Barycenter.Y = ToApproachY;
		}
			
		Barycenter.Y = Approach(Barycenter.Y, ToApproachY, 700 * DeltaTime); // 350 de base 
		Barycenter.Y = FMath::Clamp(Barycenter.Y, MinY, MaxY);
		

		float ToApproachZ = (PlayerManager->Characters[0]->GetActorLocation().Z + PlayerManager->Characters[1]->GetActorLocation().Z) / 2;

		DefaultZ = OwnerActor->GetActorLocation().Z;

		Barycenter.Z = DefaultZ;

			

		if (OwnerActor->CurrentArea) {
			if (OwnerActor->CurrentArea->ZoomMin > OwnerActor->CurrentArea->ZoomMax) { // For Some Reason, In Certain Level ZoomMin is Greater Than ZoomMax. We Manage This Case
				OffsetX = FMath::Clamp(OffsetX, OwnerActor->CurrentArea->ZoomMax, OwnerActor->CurrentArea->ZoomMin);
			}
			else {
				OffsetX = FMath::Clamp(OffsetX, OwnerActor->CurrentArea->ZoomMin, OwnerActor->CurrentArea->ZoomMax);
			}
			
		}

		Barycenter.X = Approach(Barycenter.X, OffsetX, 700 * DeltaTime);
		

	//	GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Cyan, FString::Printf(TEXT("Cam Z %f"), Barycenter.Z));
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

	if (!OwnerActor->CurrentArea) {
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
				Center.Z = character->GetActorLocation().Z;

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

				GEngine->AddOnScreenDebugMessage(-1, 5.F, FColor::Yellow, FString::Printf(TEXT("Calcul OffsetX %f"), Offset));
				
				_extendPositions.Add(FExtendData(Center, direction, _extendPositions.Num(),Offset));
			}
		}
	}
}




