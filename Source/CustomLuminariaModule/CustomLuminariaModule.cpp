#include "CustomLuminariaModule.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"

#include "UnrealEd.h"
#include "Visualizers/FMecanismComponentVisualizer.h"
#include "DiamondProject/Luminaria/ActorComponents/MecanismComponent.h"

#include "Editor.h"
#include "LevelEditorViewport.h"
#include "LevelEditor.h"
#include "SLevelViewport.h"

IMPLEMENT_GAME_MODULE(FCustomLuminariaModule, CustomLuminariaModule);

DEFINE_LOG_CATEGORY(CustomLuminariaModule)

#define LOCTEXT_NAMESPACE "CustomLuminariaModule"

FCustomLuminariaModule::FCustomLuminariaModule(){}

void FCustomLuminariaModule::StartupModule() {
    UE_LOG(CustomLuminariaModule, Warning, TEXT("CustomLuminariaModule: Log Started"));

    if (GUnrealEd) {
        TSharedPtr<FMecanismComponentVisualizer> Visualizer = MakeShareable(new FMecanismComponentVisualizer);

        if (Visualizer.IsValid()) {
            GUnrealEd->RegisterComponentVisualizer(UMecanismComponent::StaticClass()->GetFName(), Visualizer);
            Visualizer->OnRegister();
        }
    }
}

void FCustomLuminariaModule::ShutdownModule() {
    UE_LOG(CustomLuminariaModule, Warning, TEXT("CustomLuminariaModule: Log Ended"));

    if (GUnrealEd) {
        GUnrealEd->UnregisterComponentVisualizer(UMecanismComponent::StaticClass()->GetFName());
    }
}

#undef LOCTEXT_NAMESPACE