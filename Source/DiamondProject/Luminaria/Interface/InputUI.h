#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InputUI.generated.h"

UINTERFACE(MinimalAPI)
class UInputUI : public UInterface {
	GENERATED_BODY()
};


class DIAMONDPROJECT_API IInputUI {
	GENERATED_BODY()

public:
	virtual void AddInputUI() = 0;
	virtual void RemoveInputUI() = 0;
	virtual void CompleteInput() = 0;

protected:
	
	TObjectPtr<class UUIComboInput> ComboWidget;

};
