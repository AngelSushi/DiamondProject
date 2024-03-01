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

	if (_extendPositions.Num() > 0) {
		_extendPositions.RemoveAll([this, &character, &direction](const FExtendData& extendData) {
			FVector ExtendToPlayer = extendData.position - character->GetActorLocation();
			FVector Forward = extendData.direction;

			float angle = FVector::DotProduct(ExtendToPlayer, Forward);

			if (angle < 0 && Forward == -direction) {
				GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Green, TEXT("True"));

				if (ForwardDirection.X != 0) {
					_offset.X += 250.F;
				}
				else if (ForwardDirection.Y != 0) {
					_offset.Y += 250.F;
				}

				return true;
			}
			
			return false;
		});
	}
	
}


void UCameraDynamicBehavior::TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(_characters.Num() >= 2) {
		CalculateBarycenter();

		if (_barycenter == FVector::Zero())
			return;

		OwnerActor->SetActorLocation(_barycenter);
		FVector LerpVector = FMath::Lerp(FVector::Zero(),_offset, DeltaTime);

		GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Yellow, FString::Printf(TEXT("Offset %s"), *_offset.ToString()));
	//	GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Red, FString::Printf(TEXT("Distance %f"), FVector::Distance(_barycenter, _barycenter + _offset)));

		OwnerActor->AddActorLocalOffset(-LerpVector);

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
			FVector Center = character->GetActorLocation() - direction * 200.F;
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
				}
				
				_extendPositions.Add(FExtendData(Center, direction, OwnerActor->GetActorLocation()));
			}
		}
	}
}




