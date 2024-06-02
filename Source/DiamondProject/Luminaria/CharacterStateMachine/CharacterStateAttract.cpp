#include "CharacterStateAttract.h"
#include "CharacterStateIdle.h"
#include "../Core/DiamondProjectCharacter.h"
#include "../SubSystems/AbsorberEventsDispatcher.h"
#include "../Actors/Absorber.h"

#include "../SubSystems/UISubsystem.h"
#include "../UMG/UIAbsorberInput.h"
#include "../DataAssets/UIDataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "../SubSystems/InputUIManager.h"

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
		ComboWidget = CreateWidget<UUIAbsorberInput>(GetCharacter()->GetWorld(), UISystem->GetUIAsset()->AbsorberInputClass);
		TArray<TEnumAsByte<EInput>> Inputs;
		Inputs.Add(CurrentAbsorber->GetCurrentInput());

		if (!ComboWidget) {
			GEngine->AddOnScreenDebugMessage(-1, 5.F, FColor::Purple, TEXT("Error Creating Widget"));
			return;
		}

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

		ComboWidget->GetDynamicMaterial()->SetScalarParameterValue("Threshold",(float)(SpamTimer / SpamMaxTimer));

		if (GetCharacter()->GetWorld()->GetTimeSeconds() - TimerCountdown > 0.3F) {
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

	FVector Direction = (CurrentAbsorber->GetActorLocation() - DetectedCharacter->GetActorLocation());
	Direction.Normalize();

	GetCharacter()->AddMovementInput(Direction,CurrentAbsorber->GetAbsorberForce());
}

void UCharacterStateAttract::OnStateExit() {
	if (ComboWidget) {
		ComboWidget->RemoveFromViewport();
		ComboWidget = nullptr;
	}
}

void UCharacterStateAttract::OnDetectPlayer(ADiamondProjectCharacter* Character, AAbsorber* Absorber) { // Called twice but not important 
	DetectedCharacter = Character;
	CurrentAbsorber = Absorber;
}

void UCharacterStateAttract::OnUnDetectPlayer(ADiamondProjectCharacter* Character, AAbsorber* Absorber) {
	ChangeState(GetStateMachine()->StateIdle);
}

void UCharacterStateAttract::OnJump() {}

void UCharacterStateAttract::OnAbsorberInputStarted(FKey Key) {
	if (UInputUIManager::ConvertKeyToInput(Key) == CurrentAbsorber->GetCurrentInput()) {
		bIsSpam = true;
		TimerCountdown = GetCharacter()->GetWorld()->GetTimeSeconds();
	}
}