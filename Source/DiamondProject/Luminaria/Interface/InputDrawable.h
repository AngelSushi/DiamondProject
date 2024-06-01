#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InputDrawable.generated.h"

class UInputUI;

UINTERFACE(MinimalAPI)
class UInputDrawable : public UInterface {
	GENERATED_BODY()
};


class DIAMONDPROJECT_API IInputDrawable {
	GENERATED_BODY()

	
public:

	virtual void EnableInputListener() = 0;
	virtual void DisableInputListener() = 0;
	virtual void CompleteInput(UInputUI* Input) = 0;
	
protected:
	TObjectPtr<class UInputUIManager> InputUIManager;
	TObjectPtr<class UUISubsystem> UISystem;
	TObjectPtr<class UUIComboInput> ComboWidget;
};
