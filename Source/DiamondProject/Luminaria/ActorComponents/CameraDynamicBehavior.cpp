#include "CameraDynamicBehavior.h"

#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"
#include "DiamondProject/Luminaria/Actors/LuminariaCamera.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerEventsDispatcher.h"


UCameraDynamicBehavior::UCameraDynamicBehavior() {

	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UCameraDynamicBehavior::BeginPlay() {
	Super::BeginPlay();
	
	UPlayerEventsDispatcher* PlayerEventsDispatcher = GetWorld()->GetSubsystem<UPlayerEventsDispatcher>();
	PlayerEventsDispatcher->OnPlayerRegister.AddDynamic(this,&UCameraDynamicBehavior::OnRegisterPlayer);
	PlayerEventsDispatcher->OnPlayerMove.AddDynamic(this, &UCameraDynamicBehavior::OnPlayerMove);

	_canExtend = true;

	OffsetX = _defaultY;
	_barycenter.X = OffsetX;
}

void UCameraDynamicBehavior::OnRegisterPlayer(ADiamondProjectCharacter* player) {
	_characters.Add(player);
}

void UCameraDynamicBehavior::OnPlayerMove(ADiamondProjectCharacter* character, FVector direction, bool& isCanceled) {
	CalculateOffsideFrustumOffset(character, direction);

	if (_extendPositions.Num() > 0) {
		_extendPositions.RemoveAll([this, &character, &direction](const FExtendData& extendData) {
			FVector ExtendToPlayer = extendData.position - character->GetActorLocation();
			FVector Forward = extendData.direction;

			float angle = FVector::DotProduct(ExtendToPlayer, Forward);

			if (angle < 0 && Forward == -direction) {
				OffsetX -= 250.F;
				return true;
			}
			
			return false;
		});
	}
	
}


void UCameraDynamicBehavior::TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(_characters.Num() >= 2) {
		GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Green, TEXT("Tick Component"));

		_barycenter.Y = (_characters[0]->GetActorLocation().Y + _characters[1]->GetActorLocation().Y) / 2;
		_barycenter.Z = (_characters[0]->GetActorLocation().Z + _characters[1]->GetActorLocation().Z) / 2;
		_barycenter.X = Approach(_barycenter.X,OffsetX,250 * DeltaTime);

		OwnerActor->SetActorLocation(_barycenter);

		if (bDebug) {
			for (auto& extendPosition : _extendPositions) {
				DrawDebugLine(GetWorld(), extendPosition.position, extendPosition.position + FVector::UpVector * 250.f, FColor::White, false, 1.F, 1, 2.F);
				DrawDebugLine(GetWorld(), extendPosition.position, extendPosition.position + extendPosition.direction * 150.f, FColor::Green, false, 1.F, 1, 2.F);
			}
		}
	}

}

void UCameraDynamicBehavior::CalculateOffsideFrustumOffset(ADiamondProjectCharacter* character,FVector direction) {
	ULocalPlayer* LocalPlayer = character->GetWorld()->GetFirstLocalPlayerFromController();

	if (LocalPlayer != nullptr && LocalPlayer->ViewportClient != nullptr && LocalPlayer->ViewportClient->Viewport) {
		FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(LocalPlayer->ViewportClient->Viewport, character->GetWorld()->Scene, LocalPlayer->ViewportClient->EngineShowFlags).SetRealtimeUpdate(true));

		FVector ViewLocation;
		FRotator ViewRotation;
		FSceneView* SceneView = LocalPlayer->CalcSceneView(&ViewFamily, ViewLocation, ViewRotation, LocalPlayer->ViewportClient->Viewport);

		if (SceneView != nullptr) {
			FVector Center = character->GetActorLocation() - direction * 200.F - FVector(FMath::Abs(OffsetX - OwnerActor->GetActorLocation().X), 0, 0);
			
			if (bDebug) {
				DrawDebugSphere(GetWorld(), Center, character->GetSimpleCollisionRadius(), 8, FColor::Red, false, 1.F, 1, 3.F);
			}

			if (!SceneView->ViewFrustum.IntersectSphere(Center, character->GetSimpleCollisionRadius()) && _canExtend) {
				OffsetX += 250.F;

				for (auto& extendPosition : _extendPositions) {
					if (FVector::Distance(extendPosition.position, Center) < 250.F) {
						return;
					}
				}
				
				_extendPositions.Add(FExtendData(Center, direction, OwnerActor->GetActorLocation()));
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




