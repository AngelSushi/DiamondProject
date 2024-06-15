#include "DiamondProject/Luminaria/CameraBehaviors/HeightCameraBehavior.h"

#include "DiamondProject/Luminaria/SubSystems/PlayerManager.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectPlayerController.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"

#include "DiamondProject/Luminaria/Actors/LuminariaCamera.h"
#include "DiamondProject/Luminaria/Actors/CameraArea.h"

#include "Kismet/GameplayStatics.h"

// Behavior that Control Z Axis Of the Camera. 


// Un Height Dynamic qui met juste un pooint entre les deux 
// Un height static qui attends qu'il y a les deux pour faire une montée progressive

void UHeightCameraBehavior::BeginBehavior(ALuminariaCamera* Owner) {
	Super::BeginBehavior(Owner);

	PlayerManager->OnPlayerLandOnGround.AddDynamic(this, &UHeightCameraBehavior::OnPlayerLandOnGround);

	OffsetZ = OwnerActor->GetActorLocation().Z;
}

void UHeightCameraBehavior::TickBehavior(float DeltaTime) {
	Super::TickBehavior(DeltaTime);

	if (PlayerManager->Characters.Num() >= 2) {
		//if (LinePositionTop == FVector::Zero() /* || bChangeLimit*/) {
		FVector2D ViewportSize;
		GEngine->GameViewport->GetViewportSize(ViewportSize);

		FVector WorldPosition;
		FVector WorldDirection;

		PlayerManager->Characters[1]->GetLuminariaController()->DeprojectScreenPositionToWorld(0,100.F, WorldPosition, WorldDirection);

		LinePositionTop = WorldPosition + (WorldDirection * FVector::Distance(OwnerActor->GetActorLocation(), PlayerManager->Characters[1]->GetActorLocation()));

		PlayerManager->Characters[1]->GetLuminariaController()->DeprojectScreenPositionToWorld(ViewportSize.X,ViewportSize.Y - 100.F, WorldPosition, WorldDirection);
		
		LinePositionBot = WorldPosition + (WorldDirection * FVector::Distance(OwnerActor->GetActorLocation(),PlayerManager->Characters[1]->GetActorLocation()));
		bChangeLimit = false;

		if (OwnerActor->bDebugCamera) {
			DrawDebugLine(OwnerActor->GetWorld(), LinePositionTop + FVector::LeftVector * 5000000.F, LinePositionTop + FVector::RightVector * 5000000.F, FColor::White, false, 1.F, 1, 2.F);
			DrawDebugLine(OwnerActor->GetWorld(), LinePositionBot + FVector::LeftVector * 5000000.F, LinePositionBot + FVector::RightVector * 5000000.F, FColor::White, false, 1.F, 1, 2.F);
		}

		FVector HeightCameraPosition = OwnerActor->GetActorLocation();

		if (!OwnerActor->CurrentArea) {
			return;
		}

		//GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Green, FString::Printf(TEXT("HeightMin %i"), OwnerActor->CurrentArea->HeightMin));
		//GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Yellow, FString::Printf(TEXT("HeightMax %i"), OwnerActor->CurrentArea->HeightMax));

		HeightCameraPosition.Z = Approach(HeightCameraPosition.Z, OffsetZ, 700 * DeltaTime);
		HeightCameraPosition.Z = FMath::Clamp(HeightCameraPosition.Z,OwnerActor->CurrentArea->HeightMin,OwnerActor->CurrentArea->HeightMax);

	//	GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Red, FString::Printf(TEXT("Pos %s"),*HeightCameraPosition.ToString()));
			 
		OwnerActor->SetActorLocation(HeightCameraPosition);
	}
}

void UHeightCameraBehavior::OnPlayerLandOnGround(ADiamondProjectCharacter* Character) {
	if (Character->GetActorLocation().Z >= LinePositionTop.Z) {
		ADiamondProjectCharacter* OtherCharacter = PlayerManager->GetOtherPlayer(Character);

		if (!OtherCharacter) {
			return;
		}

		FVector CharacterPosition = Character->GetActorLocation();
		FVector OtherCharacterPosition = OtherCharacter->GetActorLocation();

		if (Character->GetActorLocation().Z >= OtherCharacter->GetActorLocation().Z && ExceedCharacters.Num() == 1) {
			if (ExceedCharacters.Contains(OtherCharacter)) {
				OffsetZ = CalculateOffset(Character, OtherCharacter,CharacterPosition);
			}
		}
		else {
			OffsetZ = FMath::Abs((CharacterPosition.Z + OtherCharacterPosition.Z) / 2);

			if (!ExceedCharacters.Contains(Character)) {
				ExceedCharacters.Add(Character);
			}
		}

	}
	else if (ExceedCharacters.Contains(Character)) {
		OffsetZ = DefaultZ;		
	}

	if (Character->GetActorLocation().Z <= LinePositionBot.Z) {
		GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Orange, TEXT("[HeightCameraBehavior] Depasse La Line Bot "));
		ADiamondProjectCharacter* OtherCharacter = PlayerManager->GetOtherPlayer(Character);

		FVector CharacterPosition = Character->GetActorLocation();
		FVector OtherCharacterPosition = OtherCharacter->GetActorLocation();

		if (ExceedCharacters.Num() == 1) {
			if (ExceedCharacters.Contains(OtherCharacter)) {
				AActor* GroundDetect = Character->GetGroundActor();

				if (GroundDetect) {
					float GroundZ = GroundDetect->GetActorLocation().Z /* + GroundDetect->GetActorScale().Z * 50.F*/; // Ajoutez Offset Si besoin
					OffsetZ = OwnerActor->GetActorLocation().Z - (FMath::Abs(OwnerActor->GetActorLocation().Z - GroundZ) / 2);
					
					bChangeLimit = true;
					ExceedCharacters.Empty();
				}
			}
			else {
				float MiddleZ = FMath::Abs((OtherCharacterPosition.Z + CharacterPosition.Z) / 2);
				OffsetZ = OwnerActor->GetActorLocation().Z - FMath::Abs(OwnerActor->GetActorLocation().Z - MiddleZ);
			}
		}
		else {
			float MiddleZ = FMath::Abs((OtherCharacterPosition.Z + CharacterPosition.Z) / 2);
			OffsetZ = OwnerActor->GetActorLocation().Z - FMath::Abs(OwnerActor->GetActorLocation().Z - MiddleZ);

			if (!ExceedCharacters.Contains(Character)) {
				ExceedCharacters.Add(Character);
			}
		}
	}
	else if (ExceedCharacters.Contains(Character)) { // Not Sure ==> TO Verify
		OffsetZ = DefaultZ;
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