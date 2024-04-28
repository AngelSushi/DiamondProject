#include "DiamondProject/Luminaria/CameraBehaviors/HeightCameraBehavior.h"

#include "DiamondProject/Luminaria/SubSystems/PlayerManager.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectPlayerController.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"

#include "DiamondProject/Luminaria/Actors/LuminariaCamera.h"
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
		if (LinePositionTop == FVector::Zero() || bChangeLimit) {
			LinePositionTop = PlayerManager->Characters[0]->GetActorLocation();
			LinePositionTop.Z += 650.F;
			LinePositionBot = PlayerManager->Characters[0]->GetActorLocation();
			LinePositionBot.Z -= 175.F;

			bChangeLimit = false;
		}

		if (OwnerActor->bDebugCamera) {
			DrawDebugLine(OwnerActor->GetWorld(), LinePositionTop + FVector::LeftVector * 50000.F, LinePositionTop + FVector::RightVector * 50000.F, FColor::White, false, 1.F, 1, 2.F);
			DrawDebugLine(OwnerActor->GetWorld(), LinePositionBot + FVector::LeftVector * 50000.F, LinePositionBot + FVector::RightVector * 50000.F, FColor::White, false, 1.F, 1, 2.F);
		}

		FVector HeightCameraPosition = OwnerActor->GetActorLocation();
		HeightCameraPosition.Z = Approach(HeightCameraPosition.Z, OffsetZ, 350 * DeltaTime);
		HeightCameraPosition.Z = FMath::Clamp(HeightCameraPosition.Z,OwnerActor->HeightMin,OwnerActor->HeightMax);
			 
		OwnerActor->SetActorLocation(HeightCameraPosition);
	}
}

void UHeightCameraBehavior::OnPlayerLandOnGround(ADiamondProjectCharacter* Character) {
	if (Character->GetActorLocation().Z >= LinePositionTop.Z) {
		ADiamondProjectCharacter* OtherCharacter = PlayerManager->GetOtherPlayer(Character);

		FVector CharacterPosition = Character->GetActorLocation();
		FVector OtherCharacterPosition = OtherCharacter->GetActorLocation();

		if (Character->GetActorLocation().Z >= OtherCharacter->GetActorLocation().Z && ExceedCharacters.Num() == 1) {
			if (ExceedCharacters.Contains(OtherCharacter)) {
				OffsetZ = CalculateOffset(Character, OtherCharacter,CharacterPosition);
				GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Cyan, FString::FromInt(OffsetZ));
			}
		}
		else {
			OffsetZ = FMath::Abs((CharacterPosition.Z + OtherCharacterPosition.Z) / 2);

			if (!ExceedCharacters.Contains(Character)) {
				ExceedCharacters.Add(Character);
			}
		}

	}

	if (Character->GetActorLocation().Z <= LinePositionBot.Z) {
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

			GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Black, FString::FromInt(DistanceZ));

			bChangeLimit = true;
			ExceedCharacters.Empty();

			return OwnerActor->GetActorLocation().Z + DistanceZ - Character->GetGroundActor()->GetActorScale().Z * 50.F; // 50 is Not sure maybe have to determinate this value
		}
	}

	return OwnerActor->GetActorLocation().Z;
}