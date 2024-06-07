#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIComboInput.generated.h"

UENUM(BlueprintType)
enum EInput {
	NO_INPUT,
	A,
	B,
	X,
	Y,
	JOYSTICK_R,
	JOYSTICK_L,
	RB,
	RT,
	LB,
	LT
};

class UHorizontalBox;
struct FInputData;

UCLASS()
class DIAMONDPROJECT_API UUIComboInput : public UUserWidget {
	GENERATED_BODY()

public:

	UUIComboInput(const FObjectInitializer& Initializer);

	virtual void InitComboUI(TArray<TEnumAsByte<EInput>> Inputs, FText ActionName = FText::FromString(TEXT("")));
	virtual void AddElement(TEnumAsByte<EInput> Input, UHorizontalBox* HorizontalBox);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UCanvasPanel> Canvas;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> Box;

	UFUNCTION()
	UMaterialInstanceDynamic* GetDynamicMaterial() { return DynamicMaterial; }
	
private:
	void AddText(FText Text, UHorizontalBox* HorizontalBox,float Size);

	FInputData GetDataByInput(TEnumAsByte<EInput> Input);

	UPROPERTY()
	TObjectPtr<class UUIInputDataAsset> InputsIconAsset;

protected:
	UPROPERTY()
	TObjectPtr<class UMaterial> IconMaterial;

	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> DynamicMaterial;

	UPROPERTY()
	TObjectPtr<class UFont> TextFont;

	UPROPERTY()
	FVector2D Offset;

	UPROPERTY()
	FVector2D Scale;

};