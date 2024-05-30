#pragma once

#include "CoreMinimal.h"
#include "UIComboInput.h"
#include "UIAbsorberInput.generated.h"


UCLASS()
class DIAMONDPROJECT_API UUIAbsorberInput : public UUIComboInput {
	GENERATED_BODY()

public:
	UUIAbsorberInput(const FObjectInitializer& Initializer);
	

	virtual void InitComboUI(TArray<TEnumAsByte<EInput>> Inputs, FText ActionName = FText::FromString(TEXT(""))) override;
	virtual void AddElement(TEnumAsByte<EInput> Input, UHorizontalBox* HorizontalBox) override;

private:
	UPROPERTY()
	TObjectPtr<UMaterial> NormalMaterial;
};
