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
		if (LinePosition == FVector::Zero() || bChangeLimit) {
			LinePosition = PlayerManager->Characters[0]->GetActorLocation();
			LinePosition.Z += 650.F;
			bChangeLimit = false;
		}

		DrawDebugLine(OwnerActor->GetWorld(), LinePosition + FVector::LeftVector * 50000.F, LinePosition + FVector::RightVector * 50000.F, FColor::White,false,1.F,1,2.F);

		for (ADiamondProjectCharacter* Character : PlayerManager->Characters) {

			FVector CharacterPosition = Character->GetActorLocation();

			if (CharacterPosition.Z >= LinePosition.Z) { // Check when player exceed the up limit
				if (!ExceedCharacters.Contains(Character)) {
					ExceedCharacters.Add(Character);
					GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Orange, TEXT("Add Player"));
				}
			}
			else {
				if (ExceedCharacters.Contains(Character)) {
					ExceedCharacters.Remove(Character);
				}
			}

		}

		FVector HeightCameraPosition = OwnerActor->GetActorLocation();
		HeightCameraPosition.Z = Approach(HeightCameraPosition.Z, OffsetZ, 350 * DeltaTime);

		//GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Yellow, FString::Printf(TEXT("HeightCamPos %s"), *HeightCameraPosition.ToString()));

		OwnerActor->SetActorLocation(HeightCameraPosition);

	}
}

void UHeightCameraBehavior::OnPlayerLandOnGround(ADiamondProjectCharacter* Character) {
	if (ExceedCharacters.Contains(Character)) {

		ADiamondProjectCharacter* OtherCharacter = PlayerManager->GetOtherPlayer(Character);

		FVector CharacterPosition = Character->GetActorLocation();
		FVector OtherCharacterPosition = OtherCharacter->GetActorLocation();

		if (Character->GetActorLocation().Z >= OtherCharacter->GetActorLocation().Z && ExceedCharacters.Num() == 2) {
			AActor* GroundActor = Character->GetGroundActor();

			if (GroundActor) {
				check(Character->GetWorld());

				FVector Start = GroundActor->GetActorLocation();
				FVector End = GroundActor->GetActorLocation() + FVector::DownVector * 500000.F;

				TArray<FHitResult> HitResults;
				FCollisionQueryParams Params;

				Params.AddIgnoredActor(GroundActor);
				Params.AddIgnoredActor(Character);
				Params.AddIgnoredActor(OtherCharacter);
				Params.AddIgnoredActor(OwnerActor->CurrentArea);

				Character->GetWorld()->LineTraceMultiByChannel(HitResults, Start, End, ECC_GameTraceChannel2, Params);
				
				if (HitResults.Num() > 0) {

					GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Magenta, FString::Printf(TEXT("Name %s"), *HitResults[0].GetActor()->GetActorNameOrLabel()));

					FVector GroundPosition = HitResults[0].GetActor()->GetActorLocation();
					float DistanceZ = FMath::Abs(GroundActor->GetActorLocation().Z - GroundPosition.Z);
					OffsetZ = OwnerActor->GetActorLocation().Z + DistanceZ - Character->GetGroundActor()->GetActorScale().Z * 50.F; // 50 is Not sure maybe have to determinate this value
					bChangeLimit = true;
					ExceedCharacters.Empty();
				}

				
			}
		}
		else {
			OffsetZ = FMath::Abs((CharacterPosition.Z + OtherCharacterPosition.Z) / 2);
		}

		LastHitGroundActor = Character->GetGroundActor();
	}
}
