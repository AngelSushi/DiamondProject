
#include "CameraBehavior.h"
#include "DiamondProject/Luminaria/Actors/LuminariaCamera.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerManager.h"
#include "DiamondProject/Luminaria/Actors/CameraArea.h"

#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"

#include "../SubSystems/PlayerManager.h"

void UCameraBehavior::BeginBehavior(ALuminariaCamera* Owner) {
	PlayerManager = Owner->GetWorld()->GetSubsystem<UPlayerManager>();
	PlayerManager->OnPlayerMove.AddDynamic(this, &UCameraBehavior::OnPlayerMove);

	OwnerActor = Owner;

	DefaultX = OwnerActor->CurrentArea ? OwnerActor->CurrentArea->ZoomMin : OwnerActor->GetActorLocation().X;
	DefaultZ = PlayerManager->Characters.Num() > 0 ? PlayerManager->Characters[0]->GetGroundActor()->GetActorLocation().Z : OwnerActor->GetActorLocation().Z; // Ou au ground 
	//DefaultZ = OwnerActor->GetActorLocation().Z;
}

void UCameraBehavior::TickBehavior(float DeltaTime) {
	if (!OwnerActor->CurrentArea || PlayerManager->GetAllCharactersRef().Num() < 2) {
		return;
	}

	FVector CameraCenterPosition = OwnerActor->GetActorLocation();
	CameraCenterPosition.X = PlayerManager->GetOnePlayer()->GetActorLocation().X;

	FVector LeftCorner = CalculateMaxFrustum(PlayerManager->GetOnePlayer(), CameraCenterPosition, -1.F);
	FVector RightCorner = CalculateMaxFrustum(PlayerManager->GetOnePlayer(), CameraCenterPosition, 1.F);

	float Distance = FMath::Abs(LeftCorner.Y - RightCorner.Y);
	float HalfDistance = Distance / 2;

	FVector NewMinPosition = OwnerActor->CurrentArea->MinPosition + FVector(0, HalfDistance, 0);
	FVector NewMaxPosition = OwnerActor->CurrentArea->MaxPosition - FVector(0, HalfDistance, 0);

	MinY = NewMinPosition.Y;
	MaxY = NewMaxPosition.Y;

	if (OwnerActor->bDebugCamera) {
		DrawDebugSphere(OwnerActor->GetWorld(), OwnerActor->CurrentArea->MinPosition, 128.F, 8, FColor::Green);
		DrawDebugSphere(OwnerActor->GetWorld(), OwnerActor->CurrentArea->MaxPosition, 128.F, 8, FColor::Green);

		DrawDebugSphere(OwnerActor->GetWorld(), LeftCorner, 32.F, 24.F, FColor::Cyan);
		DrawDebugSphere(OwnerActor->GetWorld(), RightCorner, 64.F, 24.F, FColor::Magenta);
	}

	
}

FVector UCameraBehavior::CalculateMaxFrustum(ADiamondProjectCharacter* Character,FVector Position,float Direction) {
	ULocalPlayer* LocalPlayer = Character->GetWorld()->GetFirstLocalPlayerFromController();

	if (LocalPlayer != nullptr && LocalPlayer->ViewportClient != nullptr && LocalPlayer->ViewportClient->Viewport) {
		FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(LocalPlayer->ViewportClient->Viewport, Character->GetWorld()->Scene, LocalPlayer->ViewportClient->EngineShowFlags).SetRealtimeUpdate(true));

		FVector ViewLocation;
		FRotator ViewRotation;
		FSceneView* SceneView = LocalPlayer->CalcSceneView(&ViewFamily, ViewLocation, ViewRotation, LocalPlayer->ViewportClient->Viewport);

		while (SceneView->ViewFrustum.IntersectPoint(Position)) {//  PAS OPTI 
			Position.Y += 50.F * Direction;
		}
	}

	return Position;
}


void UCameraBehavior::CalculateBarycenter() {
	if (PlayerManager->GetAllCharactersRef().Num() >= 2) {
		FVector First = PlayerManager->GetAllCharactersRef()[0]->GetActorLocation();
		FVector Second = PlayerManager->GetAllCharactersRef()[1]->GetActorLocation();

		float divider = 2.F;

		Barycenter = (First + Second) / divider;

		Barycenter += FVector(0, 0, 45.F);
		Barycenter.X = DefaultX;
	}
}

void UCameraBehavior::OnPlayerMove(ADiamondProjectCharacter* character, FVector2D Input, FVector direction, bool& isCanceled) {}

float UCameraBehavior::Approach(float Current, float Target, float Incr) {
	if (Current < Target) {
		return FMath::Min(Current + Incr, Target);
	}
	else if (Current > Target) {
		return FMath::Max(Current - Incr, Target);
	}
	else
		return Target;
}

