#include "CharacterStateReplacePosition.h"
#include "../Core/DiamondProjectCharacter.h"

void UCharacterStateReplacePosition::OnStateBegin() {
	Super::OnStateBegin();

	OriginX = GetCharacter()->GetActorLocation().X;
}

void UCharacterStateReplacePosition::OnStateTick(float DeltaTime) {
	Super::OnStateTick(DeltaTime);

	FVector CharacterPosition = GetCharacter()->GetActorLocation();

	GetCharacter()->SetActorLocation(FVector(OriginX, CharacterPosition.Y, CharacterPosition.Z));
}
