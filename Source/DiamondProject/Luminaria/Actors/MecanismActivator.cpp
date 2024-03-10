#include "DiamondProject/Luminaria/Actors/MecanismActivator.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Engine/GameViewportClient.h"

#include "Components/SplineComponent.h"
#include "ProceduralMeshComponent.h"
#include "Components/BoxComponent.h"

#include "DiamondProject/Luminaria/ActorComponents/MecanismComponent.h"

AMecanismActivator::AMecanismActivator() {
 	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = root;

	mesh = CreateDefaultSubobject<UMecanismComponent>(TEXT("Mecanism"));
	mesh->SetupAttachment(root);

	boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	boxCollision->SetupAttachment(RootComponent);


	GLog->Log("Log of Mecanism Activator");
}

void AMecanismActivator::OnConstruction(const FTransform& Transform) {
	FVector Loc = Transform.GetLocation();
}

void AMecanismActivator::Tick(float DeltaTime)
{
	if (WITH_EDITOR) {
		//GLog->Log("tick in actor");
	}

}

bool AMecanismActivator::ShouldTickIfViewportsOnly() const
{
	return true;
}