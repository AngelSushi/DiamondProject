#include "DiamondProject/Luminaria/ActorComponents/DebugCameraBehaviorComponent.h"
#include "DiamondProject/Luminaria/Actors/LuminariaCamera.h"

UDebugCameraBehaviorComponent::UDebugCameraBehaviorComponent() {

	PrimaryComponentTick.bCanEverTick = true;
}

void UDebugCameraBehaviorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType,ThisTickFunction);

	ComponentsList.Empty();

	for (UActorComponent* Component : OwnerActor->GetComponents()) {
		if (Component->IsA(UCameraBehavior::StaticClass()) && !Component->IsA(UDebugCameraBehaviorComponent::StaticClass())) {
			ComponentsList.Add(Component);
		}
	}

}
