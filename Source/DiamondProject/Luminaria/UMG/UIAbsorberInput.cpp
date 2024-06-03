#include "UIAbsorberInput.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"

UUIAbsorberInput::UUIAbsorberInput(const FObjectInitializer& Initializer) : Super(Initializer) {
	NormalMaterial = IconMaterial;
	IconMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Game/Luminaria/Materials/M_AbsorberInput"));

}

void UUIAbsorberInput::InitComboUI(TArray<TEnumAsByte<EInput>> Inputs, FText ActionName) {
	Super::InitComboUI(Inputs,ActionName);

	GEngine->AddOnScreenDebugMessage(-1, 5.F, FColor::Orange, TEXT("Absorber Input"));
}

void UUIAbsorberInput::AddElement(TEnumAsByte<EInput> Input, UHorizontalBox* HorizontalBox) {
	Super::AddElement(Input, HorizontalBox);

	/*UImage* IconImage = NewObject<UImage>(this, UImage::StaticClass());

	UMaterialInstanceDynamic* MaterialImage = UMaterialInstanceDynamic::Create(NormalMaterial, this);
	MaterialImage->SetScalarParameterValue(FName("UOffset"), Offset.X);
	MaterialImage->SetScalarParameterValue(FName("VOffset"), Offset.Y);
	MaterialImage->SetScalarParameterValue(FName("UScale"), Scale.X);
	MaterialImage->SetScalarParameterValue(FName("VScale"), Scale.Y);
	IconImage->SetBrushFromMaterial(MaterialImage);

	IconImage->Brush.ImageSize = FVector2D(48, 48);

	HorizontalBox->AddChild(IconImage);
	*/
	//UHorizontalBoxSlot* SlotImage = HorizontalBox->AddChildToHorizontalBox(IconImage);
	//SlotImage->SetPadding(FMargin(10, 0, 10, 0));
}
