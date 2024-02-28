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
	PlayerEventsDispatcher->OnPlayerMove.AddDynamic(this, &UCameraDynamicBehavior::OnPlayerMove);

	_canExtend = true;
}

void UCameraDynamicBehavior::OnRegisterPlayer(ADiamondProjectCharacter* player)
{
	_characters.Add(player);
}

void UCameraDynamicBehavior::OnPlayerMove(ADiamondProjectCharacter* character, FVector direction, bool& isCanceled)
{
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
				GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Black, TEXT("Agrandir la camera"));

				if (ForwardDirection.X != 0) {
					_offset.X += 250.F;
				}
				else if (ForwardDirection.Y != 0) {
					_offset.Y += 250.F;
				}

				for (auto& extendPosition : _extendPositions) {
					if (FVector::Distance(extendPosition.Key, Center) < 250.F) {
						return;
					}
				}


				_extendPositions.Add(Center, direction);

			/*	if (_canExtend) {
					_canExtend = false;

					FTimerHandle ExtendTimer;
					GetWorld()->GetTimerManager().SetTimer(ExtendTimer, [this,&ExtendTimer]() {
						GetWorld()->GetTimerManager().ClearTimer(ExtendTimer);
						_canExtend = true;
					}, 5.F, false);
					
				}*/
			}

		}
	}
}


void UCameraDynamicBehavior::TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(_characters.Num() >= 2) {
		CalculateBarycenter();

		if (_barycenter == FVector::Zero())
			return;

		OwnerActor->SetActorLocation(_barycenter);

		FVector LerpVector = FMath::Lerp(OwnerActor->GetActorLocation(),_offset,DeltaTime);
		OwnerActor->SetActorLocation(LerpVector);

		for (auto& extendPosition : _extendPositions) {
			DrawDebugLine(GetWorld(), extendPosition.Key, extendPosition.Key + FVector::UpVector * 250.f,FColor::White,false,1.F,1,2.F);
		}
	}

}


