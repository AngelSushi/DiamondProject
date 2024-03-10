#include "CustomLuminariaModule.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"

IMPLEMENT_GAME_MODULE(FCustomLuminariaModule, CustomLuminariaModule);

DEFINE_LOG_CATEGORY(CustomLuminariaModule)

#define LOCTEXT_NAMESPACE "CustomLuminariaModule"

FCustomLuminariaModule::FCustomLuminariaModule(){}

void FCustomLuminariaModule::StartupModule()
{
    UE_LOG(CustomLuminariaModule, Warning, TEXT("CustomLuminariaModule: Log Started"));
}

void FCustomLuminariaModule::ShutdownModule()
{
    UE_LOG(CustomLuminariaModule, Warning, TEXT("CustomLuminariaModule: Log Ended"));
}

#undef LOCTEXT_NAMESPACE