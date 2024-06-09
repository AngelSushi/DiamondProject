#include "MovableObject.h"
#include "../SubSystems/UISubsystem.h"
#include "../SubSystems/InputUIManager.h"

#include "../DataAssets/UIDataAsset.h"

#include "../UMG/UIComboInput.h"
#include "Components/HorizontalBox.h"

#include "../Interface/InputUI.h"

AMovableObject::AMovableObject() {
	PrimaryActorTick.bCanEverTick = true;
}


void AMovableObject::BeginPlay() {
	Super::BeginPlay();

	UISystem = GetWorld()->GetSubsystem<UUISubsystem>();
	InputUIManager = GetWorld()->GetSubsystem<UInputUIManager>();

	InputUIManager->OnInputComplete.AddDynamic(this, &AMovableObject::CompleteInput);
}

void AMovableObject::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AMovableObject::EnableInputListener() {
	
	UInputUI* Input = InputUIManager->GetInputWithClass(AMovableObject::StaticClass());

	if (!Input) {
		return;
	}

	InputUIManager->EnableInput(AMovableObject::StaticClass());

	if (Input->HasCompleted()) {
		return;
	}
	
	ComboWidget = CreateWidget<UUIComboInput>(GetWorld(), UISystem->GetUIAsset()->ComboInputClass);
	ComboWidget->InitComboUI({EInput::JOYSTICK_L,EInput::RT}, FText::FromString(TEXT("POUSSER / TIRER")));

	int32 ScreenX;
	int32 ScreenY;

	GetWorld()->GetFirstPlayerController()->GetViewportSize(ScreenX, ScreenY);
	
	ScreenY -= 200.F;
	ScreenX -= 200.F;

	ComboWidget->SetPositionInViewport(FVector2D(ScreenX / 2, ScreenY / 2));
	ComboWidget->AddToViewport();
}

void AMovableObject::DisableInputListener() {
	if (!ComboWidget) {
		return;
	}

	InputUIManager->DisableInput(AMovableObject::StaticClass());
	ComboWidget->RemoveFromViewport();
}

void AMovableObject::CompleteInput(UInputUI* Input) {
	if (Input->GetClass() == AMovableObject::StaticClass() && ComboWidget) {
		ComboWidget->RemoveFromViewport();
	}
}
