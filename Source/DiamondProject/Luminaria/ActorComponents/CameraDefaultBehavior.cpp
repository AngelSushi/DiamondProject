#include "DiamondProject/Luminaria/ActorComponents/CameraDefaultBehavior.h"
#include "DiamondProject/Luminaria/Actors/LuminariaCamera.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerEventsDispatcher.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectPlayerController.h"
#include "SceneView.h"

UCameraDefaultBehavior::UCameraDefaultBehavior(){}

void UCameraDefaultBehavior::BeginPlay()
{
	Super::BeginPlay();

	UPlayerEventsDispatcher* PlayerEventsDispatcher = GetWorld()->GetSubsystem<UPlayerEventsDispatcher>();
	PlayerEventsDispatcher->OnPlayerRegister.AddDynamic(this,&UCameraDefaultBehavior::RegisterPlayer);
	PlayerEventsDispatcher->OnPlayerMove.AddDynamic(this,&UCameraDefaultBehavior::OnPlayerMove);
	
}

void UCameraDefaultBehavior::RegisterPlayer(ADiamondProjectCharacter* Character)
{
	_characters.AddUnique(Character);
}

void UCameraDefaultBehavior::OnPlayerMove(ADiamondProjectCharacter* character, FVector direction, bool& isCanceled) {

	ULocalPlayer* LocalPlayer = character->GetWorld()->GetFirstLocalPlayerFromController();
	if (LocalPlayer != nullptr && LocalPlayer->ViewportClient != nullptr && LocalPlayer->ViewportClient->Viewport) {
		FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(LocalPlayer->ViewportClient->Viewport, character->GetWorld()->Scene, LocalPlayer->ViewportClient->EngineShowFlags).SetRealtimeUpdate(true));

		FVector ViewLocation;
		FRotator ViewRotation;
		FSceneView* SceneView = LocalPlayer->CalcSceneView(&ViewFamily, ViewLocation, ViewRotation, LocalPlayer->ViewportClient->Viewport);
		if (SceneView != nullptr) {

			FVector Center = character->GetActorLocation() - direction * 125.f;
			DrawDebugSphere(GetWorld(), Center, character->GetSimpleCollisionRadius(), 8, FColor::Red, false, 1.F, 1, 3.F);

			if (!SceneView->ViewFrustum.IntersectSphere(Center, character->GetSimpleCollisionRadius())) {
				isCanceled = true;
			}
		}
	}
}

void UCameraDefaultBehavior::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (_characters.Num() >= 2) {
		CalculateBarycenter();
		
		OwnerActor->SetActorLocation(_barycenter);
	}
}








