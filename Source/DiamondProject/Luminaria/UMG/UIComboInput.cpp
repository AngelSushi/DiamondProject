#include "UIComboInput.h"
#include "../DataAssets/UIInputDataAsset.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"

UUIComboInput::UUIComboInput(const FObjectInitializer& Initializer) : Super(Initializer) {
	InputsIcon = LoadObject<UUIInputDataAsset>(nullptr, TEXT("/Game/Luminaria/DataAssets/InputsIcon"));
}

 void UUIComboInput::InitComboUI(TArray<TEnumAsByte<EInput>> Inputs) {
	//static_assert((std::is_base_of<EInput, Args>::value && ...), "All arguments must be EInput");

	//ULuminariaGameInstance* GameInstance = Cast<ULuminariaGameInstance>(GetWorld()->GetGameInstance());

	//if (GameInstance->InputsIcon) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.F, FColor::Yellow, TEXT("Inputs Set"));
	//}

	GEngine->AddOnScreenDebugMessage(-1, 5.F, FColor::Orange, TEXT("Init Combo"));

	for (EInput Input : Inputs) {
		AddElement(Input);
	}

}

void UUIComboInput::AddElement(EInput Input) {
	GEngine->AddOnScreenDebugMessage(-1, 5.F, FColor::Orange, TEXT("Add Element"));
	//UTexture2D* InputIcon = InputsIcon->InputsIcon[Input];

	//UImage* IconImage = NewObject<UImage>(this, UImage::StaticClass());
	//IconImage->SetBrushFromTexture(InputIcon);

	//Canvas->AddChild(IconImage);
}
