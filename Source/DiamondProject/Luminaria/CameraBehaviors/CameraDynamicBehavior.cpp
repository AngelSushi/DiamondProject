#include "CameraDynamicBehavior.h"

#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"
#include "DiamondProject/Luminaria/Actors/LuminariaCamera.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerEventsDispatcher.h"
#include "DiamondProject/Luminaria/Actors/CameraArea.h"

void UCameraDynamicBehavior::BeginBehavior(ALuminariaCamera* Owner) {
	Super::BeginBehavior(Owner);
	
	PlayerEventsDispatcher->OnPlayerMove.AddDynamic(this, &UCameraDynamicBehavior::OnPlayerMove);

	GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Yellow, TEXT("Begin Behavior Dynamic"));

	OffsetX = _defaultY;
	_barycenter.X = OffsetX;
}


void UCameraDynamicBehavior::OnPlayerMove(ADiamondProjectCharacter* character, FVector direction, bool& isCanceled) {
	Super::OnPlayerMove(character, direction, isCanceled);

	CalculateOffsideFrustumOffset(character, direction);

	if (_extendPositions.Num() > 0) {
		_extendPositions.RemoveAll([this, &character, &direction](const FExtendData& extendData) {
			FVector ExtendToPlayer = extendData.position - character->GetActorLocation();
			FVector Forward = extendData.direction;

			float angle = FVector::DotProduct(ExtendToPlayer, Forward);
			float Distance = FVector::Distance(extendData.position, character->GetActorLocation());

			if (angle < 0 && Forward == -direction && Distance < 250.F) {
				OffsetX += 250.F;
				return true;
			}
			
			return false;
		});
	}
	
}


void UCameraDynamicBehavior::TickBehavior(float DeltaTime) {
	Super::TickBehavior(DeltaTime);

	GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Yellow, TEXT("Tick Behavior Dynamic"));

	if(PlayerEventsDispatcher->Characters.Num() >= 2) {

		if (!bBlock) {
			_barycenter.Y = (PlayerEventsDispatcher->Characters[0]->GetActorLocation().Y + PlayerEventsDispatcher->Characters[1]->GetActorLocation().Y) / 2;
			_barycenter.Z = (PlayerEventsDispatcher->Characters[0]->GetActorLocation().Z + PlayerEventsDispatcher->Characters[1]->GetActorLocation().Z) / 2;
			_barycenter.X = Approach(_barycenter.X, OffsetX, 250 * DeltaTime);
		}

		GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Green, FString::Printf(TEXT("Barycenter %s"), *_barycenter.ToString()));
		OwnerActor->SetActorLocation(_barycenter);

		if (OwnerActor->bDebugCamera) {
			for (auto& extendPosition : _extendPositions) {
				DrawDebugLine(GetWorld(), extendPosition.position, extendPosition.position + FVector::UpVector * 250.f, FColor::White, false, 1.F, 1, 2.F);
				DrawDebugLine(GetWorld(), extendPosition.position, extendPosition.position + extendPosition.direction * 150.f, FColor::Green, false, 1.F, 1, 2.F);
			}
		}
	}
}

void UCameraDynamicBehavior::CameraBlock() {
	Super::CameraBlock();

	GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Green, TEXT("Camera Block"));
	OffsetX = _barycenter.X;
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
				DrawDebugSphere(GetWorld(), Center, character->GetSimpleCollisionRadius(), 8, FColor::Green, false, 1.F, 1, 3.F);
				DrawDebugSphere(GetWorld(), PlayerCenter, character->GetSimpleCollisionRadius(), 8, FColor::Red, false, 1.F, 1, 3.F);
			}

			if (!SceneView->ViewFrustum.IntersectSphere(Center, character->GetSimpleCollisionRadius()) && SceneView->ViewFrustum.IntersectSphere(PlayerCenter, character->GetSimpleCollisionRadius())) {
				for (auto& extendPosition : _extendPositions) {
					if (FVector::Distance(extendPosition.position, Center) < 250.F) {
						return;
					}
				}

				OffsetX -= 250.F;
				Center.Y = character->GetActorLocation().Y;
				_extendPositions.Add(FExtendData(Center, direction,_extendPositions.Num()));
			}
		}
	}
}

float UCameraDynamicBehavior::Approach(float Current, float Target, float Incr) {
	if (Current < Target) {
		return FMath::Min(Current + Incr, Target);
	}
	else if (Current > Target) {
		return FMath::Max(Current - Incr, Target);
	}
	else
		return Target;
}




