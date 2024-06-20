#include "DiamondProject/Luminaria/CameraBehaviors/HeightCameraBehavior.h"

#include "DiamondProject/Luminaria/SubSystems/PlayerManager.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectPlayerController.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"

#include "DiamondProject/Luminaria/Actors/LuminariaCamera.h"
#include "DiamondProject/Luminaria/Actors/CameraArea.h"

#include "Kismet/GameplayStatics.h"

void UHeightCameraBehavior::BeginBehavior(ALuminariaCamera* Owner) {
	Super::BeginBehavior(Owner);

	GEngine->AddOnScreenDebugMessage(-1, 5.F, FColor::Red, TEXT("Begin Behavior"));

	PlayerManager->OnPlayerMove.AddDynamic(this,&UHeightCameraBehavior::OnMovePlayer);
	PlayerManager->OnPlayerRespawn.AddDynamic(this, &UHeightCameraBehavior::OnPlayerRespawn);

	OffsetZ = OwnerActor->GetActorLocation().Z;
}

void UHeightCameraBehavior::TickBehavior(float DeltaTime) {
	Super::TickBehavior(DeltaTime);

	if (PlayerManager->Characters.Num() >= 2) {

		DetermineLimit();

		FVector HeightCameraPosition = OwnerActor->GetActorLocation();

		if (!OwnerActor->CurrentArea) {
			return;
		}


		//GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Green, FString::Printf(TEXT("OffsetZ %i"),OffsetZ));

		HeightCameraPosition.Z = Approach(HeightCameraPosition.Z, OffsetZ, 700 * DeltaTime);
		HeightCameraPosition.Z = FMath::Clamp(HeightCameraPosition.Z,OwnerActor->CurrentArea->HeightMin,OwnerActor->CurrentArea->HeightMax);

		OwnerActor->SetActorLocation(HeightCameraPosition);
	}
}

void UHeightCameraBehavior::OnMovePlayer(ADiamondProjectCharacter* Character) {
	if (Character->GetActorLocation().Z >= LinePositionTop.Z) {
		ADiamondProjectCharacter* OtherCharacter = PlayerManager->GetOtherPlayer(Character);

		if (!OtherCharacter) {
			return;
		}
		FVector CharacterPosition = Character->GetActorLocation();
		FVector OtherCharacterPosition = OtherCharacter->GetActorLocation();

		OffsetZ = FMath::Abs((CharacterPosition.Z + OtherCharacterPosition.Z) / 2);
	}

	if (Character->GetActorLocation().Z <= LinePositionBot.Z) {
		ADiamondProjectCharacter* OtherCharacter = PlayerManager->GetOtherPlayer(Character);

		FVector CharacterPosition = Character->GetActorLocation();
		FVector OtherCharacterPosition = OtherCharacter->GetActorLocation();

		float MiddleZ = FMath::Abs((OtherCharacterPosition.Z + CharacterPosition.Z) / 2);
		OffsetZ = OwnerActor->GetActorLocation().Z - FMath::Abs(OwnerActor->GetActorLocation().Z - MiddleZ);
	}
}

void UHeightCameraBehavior::OnPlayerRespawn(ADiamondProjectCharacter* Character, EDeathCause DeathCause, FVector RespawnPosition) {
	OffsetZ = 0;
}

void UHeightCameraBehavior::DetermineLimit() {
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);

	FVector WorldPosition;
	FVector WorldDirection;

	PlayerManager->Characters[1]->GetLuminariaController()->DeprojectScreenPositionToWorld(0, 100.F, WorldPosition, WorldDirection);

	LinePositionTop = WorldPosition + (WorldDirection * FVector::Distance(OwnerActor->GetActorLocation(), PlayerManager->Characters[1]->GetActorLocation()));

	PlayerManager->Characters[1]->GetLuminariaController()->DeprojectScreenPositionToWorld(ViewportSize.X, ViewportSize.Y - 100.F, WorldPosition, WorldDirection);

	LinePositionBot = WorldPosition + (WorldDirection * FVector::Distance(OwnerActor->GetActorLocation(), PlayerManager->Characters[1]->GetActorLocation()));
	bChangeLimit = false;

	if (OwnerActor->bDebugCamera) {
		DrawDebugLine(OwnerActor->GetWorld(), LinePositionTop + FVector::LeftVector * 5000000.F, LinePositionTop + FVector::RightVector * 5000000.F, FColor::White, false, 1.F, 1, 2.F);
		DrawDebugLine(OwnerActor->GetWorld(), LinePositionBot + FVector::LeftVector * 5000000.F, LinePositionBot + FVector::RightVector * 5000000.F, FColor::White, false, 1.F, 1, 2.F);
	}
}

float UHeightCameraBehavior::CalculateOffset(ADiamondProjectCharacter* Character, ADiamondProjectCharacter* Other, FVector Start) {
	AActor* GroundActor = Character->GetGroundActor();

	if (GroundActor) {
		check(Character->GetWorld()); 

		FVector End = Start + FVector::DownVector * 500000.F;

		TArray<FHitResult> HitResults;
		FCollisionQueryParams Params;

		Params.AddIgnoredActor(GroundActor);
		Params.AddIgnoredActor(Character);
		Params.AddIgnoredActor(Other);
		Params.AddIgnoredActor(OwnerActor->CurrentArea);

		Character->GetWorld()->LineTraceMultiByChannel(HitResults, Start, End, ECC_GameTraceChannel2, Params);

		if (HitResults.Num() > 0) {
			FVector GroundPosition = HitResults[0].GetActor()->GetActorLocation();
			float DistanceZ = FMath::Abs(GroundActor->GetActorLocation().Z - GroundPosition.Z);
			bChangeLimit = true;
			ExceedCharacters.Empty();

			return OwnerActor->GetActorLocation().Z + DistanceZ - Character->GetGroundActor()->GetActorScale().Z * 50.F; // 50 is Not sure maybe have to determinate this value
		}
	}

	return OwnerActor->GetActorLocation().Z;
}