#include "MovableObject.h"
#include "../SubSystems/UISubsystem.h"
#include "../SubSystems/InputUIManager.h"

#include "../DataAssets/UIDataAsset.h"

#include "../UMG/UIComboInput.h"
#include "Components/HorizontalBox.h"

AMovableObject::AMovableObject() {
	PrimaryActorTick.bCanEverTick = true;
}


void AMovableObject::BeginPlay() {
	Super::BeginPlay();

	UISystem = GetWorld()->GetSubsystem<UUISubsystem>();
	InputUIManager = GetWorld()->GetSubsystem<UInputUIManager>();
}

void AMovableObject::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AMovableObject::AddInputUI() {
	ComboWidget = CreateWidget<UUIComboInput>(GetWorld(),UISystem->GetUIAsset()->ComboInputClass);
	TArray<TEnumAsByte<EInput>> Inputs;
	Inputs.Add(EInput::A);
	Inputs.Add(EInput::B);
	ComboWidget->InitComboUI(Inputs,FText::FromString(TEXT("Pousser/Tirer")));

	int32 ScreenX;
	int32 ScreenY;

	GetWorld()->GetFirstPlayerController()->GetViewportSize(ScreenX, ScreenY);
	ScreenY -= 600.F;
	ScreenX -= 200.F;

	ComboWidget->SetPositionInViewport(FVector2D(ScreenX / 2, ScreenY / 2));
	ComboWidget->AddToViewport();
}

void AMovableObject::RemoveInputUI() {
	ComboWidget->RemoveFromViewport();
}

void AMovableObject::CompleteInput() {
	InputUIManager->CompleteInput(this);
}
