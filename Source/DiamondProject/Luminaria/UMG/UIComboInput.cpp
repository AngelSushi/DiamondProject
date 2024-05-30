#include "UIComboInput.h"
#include "../DataAssets/UIInputDataAsset.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"

UUIComboInput::UUIComboInput(const FObjectInitializer& Initializer) : Super(Initializer) {
	InputsIconAsset = LoadObject<UUIInputDataAsset>(nullptr, TEXT("/Game/Luminaria/DataAssets/InputsIcon"));
	IconMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Game/Luminaria/Materials/M_InputsIconAtlas"));
	
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.F, FColor::Blue, TEXT("Creation"));
	}
}

 void UUIComboInput::InitComboUI(TArray<TEnumAsByte<EInput>> Inputs, FText ActionName /*= FText::FromString(TEXT(""))*/) {
	for (int i = 0; i < Inputs.Num(); i++) {
		EInput Input = Inputs[i];

		if (!Box) {
			GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Red, TEXT("Error Reading Box"));
			return;
		}

		AddElement(Input, Box);

		if (i < Inputs.Num() - 1) {
			AddText(FText::FromString(TEXT("+")), Box,14);
		}
	}
	
	AddText(ActionName, Box,16);
	
}

void UUIComboInput::AddElement(TEnumAsByte<EInput> Input,UHorizontalBox* HorizontalBox) {
	FInputData TargetData = GetDataByInput(Input);

	if (TargetData.Input == EInput::NO_INPUT) {
		return;
	}
	
	Offset = TargetData.Offset;
	Scale = TargetData.Scale;

	UImage* IconImage = NewObject<UImage>(this, UImage::StaticClass());

	UMaterialInstanceDynamic* MaterialImage = UMaterialInstanceDynamic::Create(IconMaterial,this);
	MaterialImage->SetScalarParameterValue(FName("UOffset"), Offset.X);
	MaterialImage->SetScalarParameterValue(FName("VOffset"), Offset.Y);
	MaterialImage->SetScalarParameterValue(FName("UScale"), Scale.X);
	MaterialImage->SetScalarParameterValue(FName("VScale"), Scale.Y);
	IconImage->SetBrushFromMaterial(MaterialImage);

	IconImage->Brush.ImageSize = FVector2D(48, 48);
	
	HorizontalBox->AddChild(IconImage);

	UHorizontalBoxSlot* SlotImage = HorizontalBox->AddChildToHorizontalBox(IconImage);
	SlotImage->SetPadding(FMargin(10, 0, 10, 0));
}

void UUIComboInput::AddText(FText Text, UHorizontalBox* HorizontalBox, float Size) {
	UTextBlock* TextBlock = NewObject<UTextBlock>(this, UTextBlock::StaticClass());
	TextBlock->SetText(Text);

	FSlateFontInfo FontInfo = TextBlock->Font;
	FontInfo.Size = Size;
	TextBlock->SetFont(FontInfo);

	HorizontalBox->AddChild(TextBlock);

	UHorizontalBoxSlot* SlotText = HorizontalBox->AddChildToHorizontalBox(TextBlock);

	SlotText->HorizontalAlignment = HAlign_Center;
	SlotText->VerticalAlignment = VAlign_Center;
	SlotText->SetPadding(FMargin(0, 3, 0, 0));
}

FInputData UUIComboInput::GetDataByInput(TEnumAsByte<EInput> Input) {
	for (FInputData& InputData : InputsIconAsset->InputsIcon) {
		if (InputData.Input == Input) {
			return InputData;
		}
	}

	FInputData Data;
	return Data;
}
