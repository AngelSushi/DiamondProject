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
	_defaultY = OwnerActor->GetActorLocation().Y;
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

			FVector Center = character->GetActorLocation() + character->GetActorForwardVector() * 125.f;
			FVector CenterSecond = character->GetActorLocation() - direction * 125.f;
			DrawDebugSphere(GetWorld(), CenterSecond, character->GetSimpleCollisionRadius(), 8, FColor::Red, false, 1.F, 1, 3.F);

			if (!SceneView->ViewFrustum.IntersectSphere(Center, character->GetSimpleCollisionRadius()) && !SceneView->ViewFrustum.IntersectSphere(CenterSecond, character->GetSimpleCollisionRadius())) {
				GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Red, TEXT("Pas de Caméra"));
				isCanceled = true;
			}
		}
	}
}

void UCameraDefaultBehavior::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (_characters.Num() >= 2) {
		FVector First = _characters[0]->GetActorLocation();
		FVector Second = _characters[1]->GetActorLocation();

		float divider = 2.F;

		if (!_characters[0]->GetMesh()->IsVisible()) {
			First = FVector::Zero();
			divider -= 1.F;
		}

		if (!_characters[1]->GetMesh()->IsVisible()) {
			Second = FVector::Zero();
			divider -= 1.F;
		}

		FVector barycenter = (First + Second) / divider;
		barycenter += FVector(0, 0, 45.F);

		if (ForwardDirection.X != 0) {
			barycenter.X = _defaultY;
		}
		else if (ForwardDirection.Y != 0) {
			barycenter.Y = _defaultY;
		}

		OwnerActor->SetActorLocation(barycenter);

		for (ADiamondProjectCharacter* character : _characters) {
			FVector Center = character->GetActorLocation() + character->GetActorForwardVector() * 125.f;
			DrawDebugSphere(GetWorld(), Center, character->GetSimpleCollisionRadius(), 8, FColor::Yellow, false, 1.F, 1, 3.F);
		}
	}
}








