#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIComboInput.generated.h"

UENUM(BlueprintType)
enum EInput {
	A,
	B,
	X,
	Y,
	JOYSTICK_R,
	JOYSTICK_L,
};

UCLASS()
class DIAMONDPROJECT_API UUIComboInput : public UUserWidget {
	GENERATED_BODY()

public:

	UUIComboInput(const FObjectInitializer& Initializer);

	UFUNCTION()
	void InitComboUI(TArray<TEnumAsByte<EInput>> Inputs);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UCanvasPanel> Canvas;
	
private:
	void AddElement(EInput Input);

	UPROPERTY()
	TObjectPtr<class UUIInputDataAsset> InputsIcon;

};