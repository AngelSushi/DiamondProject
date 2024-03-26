#include "LuminariaCamera.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerEventsDispatcher.h"
#include "DiamondProject/Luminaria/ActorComponents/GoToBehavior.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"

#include "DiamondProject/Luminaria/ActorComponents/CameraDefaultBehavior.h"
#include "DiamondProject/Luminaria/ActorComponents/CameraLeaderBehavior.h"
#include "DiamondProject/Luminaria/ActorComponents/CameraDynamicBehavior.h"
#include "DiamondProject/Luminaria/ActorComponents/GoToBehavior.h"

ALuminariaCamera::ALuminariaCamera() {
	PrimaryActorTick.bCanEverTick = true;
}

void ALuminariaCamera::BeginPlay() {
	Super::BeginPlay();

	UPlayerEventsDispatcher* EventsDispatcher = GetWorld()->GetSubsystem<UPlayerEventsDispatcher>();
	EventsDispatcher->OnPlayerRegister.AddDynamic(this, &ALuminariaCamera::OnPlayerRegister);
	EventsDispatcher->OnPlayerDeath.AddDynamic(this, &ALuminariaCamera::OnPlayerDeath);

	StartPosition = GetActorLocation();

	LastBehavior = CameraBehavior;
}

void ALuminariaCamera::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	/*if (CameraBehavior != LastBehavior) {

		if (LastBehaviorComponent) {
			GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Red, FString::Printf(TEXT("Removed %s Component"), *LastBehaviorComponent->GetName()));
			LastBehaviorComponent->DestroyComponent();
		}
		
		switch (CameraBehavior) {
			case ECameraBehavior::DEFAULT:
				LastBehaviorComponent = AddComponentByClass(UCameraDefaultBehavior::StaticClass(),false,GetActorTransform(),false);
				GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Green, FString::Printf(TEXT("Added %s Component"), *LastBehaviorComponent->GetName()));
				break;
			
			case ECameraBehavior::LEADER:
				LastBehaviorComponent = AddComponentByClass(UCameraLeaderBehavior::StaticClass(), false, GetActorTransform(), false);
				GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Green, FString::Printf(TEXT("Added %s Component"), *LastBehaviorComponent->GetName()));
				break;

			case ECameraBehavior::DYNAMIC:
				LastBehaviorComponent = AddComponentByClass(UCameraDynamicBehavior::StaticClass(), false, GetActorTransform(), false);
				GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Green, FString::Printf(TEXT("Added %s Component"), *LastBehaviorComponent->GetName()));
				break;

			case ECameraBehavior::GOTO:
				LastBehaviorComponent = AddComponentByClass(UGoToBehavior::StaticClass(), false, GetActorTransform(), false);
				GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Green, FString::Printf(TEXT("Added %s Component"), *LastBehaviorComponent->GetName()));

				UGoToBehavior* GoToBehaviorComponent = Cast<UGoToBehavior>(LastBehaviorComponent);

				FVector GoTo = FVector::Zero();

				for (ADiamondProjectCharacter* ACharacter : Characters) {
					GoTo += ACharacter->GetActorLocation();
				}

				GoTo /= Characters.Num();
				GoTo.X = StartPosition.X;
				GoTo.Z = StartPosition.Z;

				GoToBehaviorComponent->GoTo = GoTo;
				GoToBehaviorComponent->Speed = 1.F;
				GoToBehaviorComponent->NextBehavior = LastBehavior;


				break;
		}
	}


	LastBehavior = CameraBehavior;
	*/
}

void ALuminariaCamera::OnPlayerRegister(ADiamondProjectCharacter* Character) {
	Characters.Add(Character);
}

void ALuminariaCamera::AddComponent(TSubclassOf<class UCameraBehavior> Component, TFunction<void(UActorComponent* AddedComponent)> ResultFunc) {
	if (LastBehaviorComponent) {
		LastBehaviorComponent->DestroyComponent();
	}

	LastBehaviorComponent = AddComponentByClass(Component, false, GetActorTransform(), false);

	ResultFunc(LastBehaviorComponent);
}


void ALuminariaCamera::OnPlayerDeath(ADiamondProjectCharacter* Character) {
	ECameraBehavior CurrentBehavior = CameraBehavior;

	if (CurrentBehavior == ECameraBehavior::DEFAULT) {
		return;
	}

	if (bHasDead) {
		return;
	}
	
	bHasDead = true;

	CameraBehavior = ECameraBehavior::GOTO;

	FTimerHandle Timer;
	GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Red, FString::FromInt(CurrentBehavior));

	AddComponent(UGoToBehavior::StaticClass(), [this,CurrentBehavior](UActorComponent* Component) {
		if (UGoToBehavior* GoToBehaviorComponent = Cast<UGoToBehavior>(Component)) {
			FVector GoTo = FVector::Zero();

			for (ADiamondProjectCharacter* ACharacter : Characters) {
				GoTo += ACharacter->GetActorLocation();
			}

			GoTo /= Characters.Num();
			GoTo.X = StartPosition.X;
			GoTo.Z = StartPosition.Z;

			GoToBehaviorComponent->GoTo = GoTo;
			GoToBehaviorComponent->Speed = 10.F;
			GoToBehaviorComponent->NextBehavior = CurrentBehavior;
		}
	});
}
