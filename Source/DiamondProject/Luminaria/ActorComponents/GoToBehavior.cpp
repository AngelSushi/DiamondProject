#include "DiamondProject/Luminaria/ActorComponents/GoToBehavior.h"

UGoToBehavior::UGoToBehavior() {
}

void UGoToBehavior::BeginPlay() {
	Super::BeginPlay();
}

void UGoToBehavior::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

FVector UGoToBehavior::Approach(FVector Current, FVector Target, float Incr) {
	FVector Normal = (Target - Current);
	float VecLength = Normal.Length();

	if (VecLength > Incr) {
		return Current + (Normal / VecLength) * Incr;
	}
	else {
		return Target;
	}
}