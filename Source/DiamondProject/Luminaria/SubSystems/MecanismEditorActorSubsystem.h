#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "MecanismEditorActorSubsystem.generated.h"

UCLASS(Blueprintable)
class DIAMONDPROJECT_API UMecanismEditorActorSubsystem : public UEditorActorSubsystem {
	GENERATED_BODY()
	
	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;

private:
	UFUNCTION()
	void OnPlaceActors(UObject* usedObject,const TArray<AActor*>& placedActors);

};
