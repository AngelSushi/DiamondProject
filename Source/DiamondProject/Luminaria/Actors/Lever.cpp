#include "Lever.h"
#include "../SubSystems/InputUIManager.h"
#include "../SubSystems/UISubsystem.h"


ALever::ALever() {
 	PrimaryActorTick.bCanEverTick = true;

}

void ALever::BeginPlay() {
	Super::BeginPlay();

	UISystem = GetWorld()->GetSubsystem<UUISubsystem>();
	InputUIManager = GetWorld()->GetSubsystem<UInputUIManager>();

	InputUIManager->OnInputComplete.AddDynamic(this, &ALever::CompleteInput);
}

void ALever::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ALever::EnableInputListener() {

	UInputUI* Input = InputUIManager->GetInputWithClass(ALever::StaticClass());

	if (!Input) {
		return;
	}

	InputUIManager->EnableInput(ALever::StaticClass());

	if (Input->HasCompleted()) {
		return;
	}

	ComboWidget = CreateWidget<UUIComboInput>(GetWorld(), UISystem->GetUIAsset()->ComboInputClass);
	ComboWidget->InitComboUI({ EInput::X}, FText::FromString(TEXT("INTERAGIR")));

	int32 ScreenX;
	int32 ScreenY;

	GetWorld()->GetFirstPlayerController()->GetViewportSize(ScreenX, ScreenY);

	ScreenY -= 200.F;
	ScreenX -= 200.F;

	ComboWidget->SetPositionInViewport(FVector2D(ScreenX / 2, ScreenY / 2));
	ComboWidget->AddToViewport();
}

void ALever::DisableInputListener() {
	if (!ComboWidget) {
		return;
	}

	InputUIManager->DisableInput(ALever::StaticClass());
	ComboWidget->RemoveFromViewport();
}

void ALever::CompleteInput(UInputUI* Input) {
	if (Input->GetClass() == ALever::StaticClass() && ComboWidget) {
		ComboWidget->RemoveFromViewport();
	}
}
