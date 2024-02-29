#include "CameraDynamicBehavior.h"

#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"
#include "DiamondProject/Luminaria/Actors/LuminariaCamera.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerEventsDispatcher.h"


UCameraDynamicBehavior::UCameraDynamicBehavior() {}

void UCameraDynamicBehavior::BeginPlay() {
	Super::BeginPlay();
	
	UPlayerEventsDispatcher* PlayerEventsDispatcher = GetWorld()->GetSubsystem<UPlayerEventsDispatcher>();
	PlayerEventsDispatcher->OnPlayerRegister.AddDynamic(this,&UCameraDynamicBehavior::OnRegisterPlayer);
	PlayerEventsDispatcher->OnPlayerMove.AddDynamic(this, &UCameraDynamicBehavior::OnPlayerMove);

	_canExtend = true;
}

void UCameraDynamicBehavior::OnRegisterPlayer(ADiamondProjectCharacter* player) {
	_characters.Add(player);
}

void UCameraDynamicBehavior::OnPlayerMove(ADiamondProjectCharacter* character, FVector direction, bool& isCanceled) {
	CalculateOffsideFrustumOffset(character, direction);

	GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Red, FString::Printf(TEXT("Direction %s"), *direction.ToString()));

	if (_extendPositions.Num() > 0) {
		for (FExtendData& extendData : _extendPositions) {
			FVector ExtendToPlayer = extendData.position - character->GetActorLocation();
			FVector Forward = extendData.direction;

			float angle = FVector::DotProduct(ExtendToPlayer, Forward);

			GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Blue, FString::Printf(TEXT("Angle : %f"), angle));
		
			if (angle < 0) {
				if (ForwardDirection.X != 0) {
					_offset.X -= extendData.cameraPosition.X;
				}
				else if (ForwardDirection.Y != 0) {
					_offset.Y -= extendData.cameraPosition.Y;
				}

				_extendPositions.Remove(extendData);
			}
		}
	}
}


void UCameraDynamicBehavior::TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(_characters.Num() >= 2) {
		CalculateBarycenter();

		if (_barycenter == FVector::Zero())
			return;

		OwnerActor->SetActorLocation(_barycenter);

		FVector LerpVector = FMath::Lerp(OwnerActor->GetActorLocation(),_offset,DeltaTime);
		OwnerActor->SetActorLocation(LerpVector);

		for (auto& extendPosition : _extendPositions) {
			DrawDebugLine(GetWorld(), extendPosition.position, extendPosition.position + FVector::UpVector * 250.f,FColor::White,false,1.F,1,2.F);
			DrawDebugLine(GetWorld(), extendPosition.position, extendPosition.position + extendPosition.direction * 150.f, FColor::Green, false, 1.F, 1, 2.F);
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
			FVector Center = character->GetActorLocation() - direction * 125.f;
			DrawDebugSphere(GetWorld(), Center, character->GetSimpleCollisionRadius(), 8, FColor::Red, false, 1.F, 1, 3.F);

			if (!SceneView->ViewFrustum.IntersectSphere(Center, character->GetSimpleCollisionRadius()) && _canExtend) {

				if (ForwardDirection.X != 0) {
					_offset.X += 250.F;
				}
				else if (ForwardDirection.Y != 0) {
					_offset.Y += 250.F;
				}

				for (auto& extendPosition : _extendPositions) {
					if (FVector::Distance(extendPosition.position, Center) < 250.F) {
						return;
					}

					return;
				}

				_extendPositions.Add(FExtendData(Center, direction, _offset));
			}
		}
	}
}




