#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(CustomLuminariaModule, All, All)

class FCustomLuminariaModule : public IModuleInterface {

public:
	FCustomLuminariaModule();

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};
