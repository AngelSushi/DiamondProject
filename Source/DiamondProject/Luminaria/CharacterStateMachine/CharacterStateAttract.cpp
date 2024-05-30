#include "CharacterStateAttract.h"
#include "CharacterStateIdle.h"
#include "../Core/DiamondProjectCharacter.h"
#include "../SubSystems/AbsorberEventsDispatcher.h"
#include "../Actors/Absorber.h"

#include "../SubSystems/UISubsystem.h"
#include "../UMG/UIComboInput.h"
#include "../DataAssets/UIDataAsset.h"
#include "Kismet/GameplayStatics.h"

void UCharacterStateAttract::OnStateInit() {
	Super::OnStateInit();

	AbsorberEventsDispatcher = GetCharacter()->GetWorld()->GetSubsystem<UAbsorberEventsDispatcher>();
	AbsorberEventsDispatcher->OnUnDetectPlayer.AddDynamic(this, &UCharacterStateAttract::OnUnDetectPlayer);
	AbsorberEventsDispatcher->OnDetectPlayer.AddDynamic(this, &UCharacterStateAttract::OnDetectPlayer);

	UISystem = GetCharacter()->GetWorld()->GetSubsystem<UUISubsystem>();
}

void UCharacterStateAttract::OnStateBegin() {
	Super::OnStateBegin();
}

void UCharacterStateAttract::OnStateTick(float DeltaTime) {
	
	if (!CurrentAbsorber) {
		return;
	}

	if (!ComboWidget) {
		ComboWidget = CreateWidget<UUIComboInput>(GetCharacter()->GetWorld(), UISystem->GetUIAsset()->ComboInputClass);
		TArray<TEnumAsByte<EInput>> Inputs;
		Inputs.Add(CurrentAbsorber->GetCurrentInput());
		ComboWidget->InitComboUI(Inputs);
		ComboWidget->AddToViewport();
	}
	else {
		FVector PlayerPosition = DetectedCharacter->GetActorLocation();
		FVector2D PlayerScreenPosition;

		UGameplayStatics::ProjectWorldToScreen(DetectedCharacter->GetLuminariaController(), PlayerPosition, PlayerScreenPosition);
		PlayerScreenPosition -= FVector2D(24, 24); // 24 is the width / 2 of the image
		PlayerScreenPosition -= FVector2D(10, DetectedCharacter->GetSimpleCollisionRadius() + 50.F);
		
		ComboWidget->SetPositionInViewport(PlayerScreenPosition);
	}

	if (bIsSpam) {
		SpamTimer += DeltaTime;

		if (GetWorld()->GetTimeSeconds() - TimerCountdown > 0.3F) {
			SpamTimer = 0.F;
			bIsSpam = false;
		}

		if (SpamTimer >= SpamMaxTimer) {
			SpamTimer = 0.F;
			bIsSpam = false;
			ChangeState(GetStateMachine()->StateIdle);
			AbsorberEventsDispatcher->OnStunAbsorber.Broadcast(CurrentAbsorber);
		}
	}

	GetCharacter()->AddMovementInput(FVector::RightVector,CurrentAbsorber->GetAbsorberForce());
}

void UCharacterStateAttract::OnStateExit() {
	ComboWidget->RemoveFromViewport();
	ComboWidget = nullptr;
}

void UCharacterStateAttract::OnDetectPlayer(ADiamondProjectCharacter* Character, AAbsorber* Absorber) { // Called twice but not important 
	DetectedCharacter = Character;
	CurrentAbsorber = Absorber;
}

void UCharacterStateAttract::OnUnDetectPlayer(ADiamondProjectCharacter* Character, AAbsorber* Absorber) {
	ChangeState(GetStateMachine()->StateIdle);
}

void UCharacterStateAttract::OnJump() {}

void UCharacterStateAttract::OnAbsorberInputStarted() {
	bIsSpam = true;
	TimerCountdown = GetWorld()->GetTimeSeconds();
}

