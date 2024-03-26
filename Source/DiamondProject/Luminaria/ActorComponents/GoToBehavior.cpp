#include "DiamondProject/Luminaria/ActorComponents/GoToBehavior.h"
#include "DiamondProject/Luminaria/Actors/LuminariaCamera.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerEventsDispatcher.h"

UGoToBehavior::UGoToBehavior() {
}

void UGoToBehavior::BeginPlay() {
	Super::BeginPlay();

	_barycenter = OwnerActor->GetActorLocation();

}

void UGoToBehavior::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Blue, TEXT("LOG"));


	if (!Approach(_barycenter, GoTo, Speed * DeltaTime)) {
		GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Red, FString::Printf(TEXT("GoTo %s"), *GoTo.ToString()));
		OwnerActor->SetActorLocation(_barycenter);
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Green, FString::Printf(TEXT("GoTo %s"), *GoTo.ToString()));

		OwnerActor->SetActorLocation(GoTo);
		OwnerActor->CameraBehavior = NextBehavior;
	}
}

bool UGoToBehavior::Approach(FVector& Current, FVector Target, float Incr) {
	FVector Normal = (Target - Current);
	float VecLength = Normal.Length();

	if (VecLength > Incr) {
		Current += (Normal / VecLength) * Incr;
		return false;
	}
	else {
		Current = Target;
		return true;
	}
}