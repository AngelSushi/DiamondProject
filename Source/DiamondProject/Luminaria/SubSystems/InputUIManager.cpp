#include "InputUIManager.h"
#include "PlayerManager.h"
#include "../Actors/MecanismActivator.h"

#include "Kismet/GameplayStatics.h"

void UInputUIManager::OnWorldBeginPlay(UWorld& InWorld) {
	PlayerManager = InWorld.GetSubsystem<UPlayerManager>();
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UInputUI::StaticClass(), ActorsInputUI);
}

void UInputUIManager::OnPlayerMove() {

	for (int i = 0; i < ActorsInputUI.Num(); i++) {
		AActor* ActorInputUI =ActorsInputUI[i];
		IInputUI* InputUIInterface = Cast<IInputUI>(ActorInputUI);

		if (CompletedInputs.Contains(InputUIInterface)) {
			continue;
		}

		if (ActorInputUI->IsA(AActor::StaticClass())) {
			ADiamondProjectCharacter* FirstCharacter = PlayerManager->GetAllCharactersRef()[0];
			ADiamondProjectCharacter* SecondCharacter = PlayerManager->GetAllCharactersRef()[1];

			float FirstDistanceSqr = FVector::DistSquared(FirstCharacter->GetActorLocation(), ActorInputUI->GetActorLocation());
			float SecondDistanceSqr = FVector::DistSquared(FirstCharacter->GetActorLocation(), ActorInputUI->GetActorLocation());

			if ((FirstDistanceSqr < 225.F * 225.F || SecondDistanceSqr < 225.F * 225.F)) {
				if (ActivatedActors.Contains(ActorInputUI)) {
					continue;
				}

				InputUIInterface->AddInputUI();
				ActivatedActors.Add(ActorInputUI);
			}
			else {
				if (ActivatedActors.Contains(ActorInputUI)) {
					ActivatedActors.Remove(ActorInputUI);
					InputUIInterface->RemoveInputUI();
				}
			}
			
		}
	}
}

void UInputUIManager::CompleteInput(IInputUI* InputUI) {
	CompletedInputs.Add(InputUI);
}
