#include "DiamondProject/Luminaria/Actors/FallingPlateform.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"
// Sets default values
AFallingPlateform::AFallingPlateform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	RootComponent = PlatformMesh;

	DetectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	DetectionBox->SetupAttachment(RootComponent);
	DetectionBox->OnComponentBeginOverlap.AddDynamic(this, &AFallingPlateform::OnCharacterOverlapBegin);
	DetectionBox->OnComponentEndOverlap.AddDynamic(this, &AFallingPlateform::OnCharacterOverlapEnd);

	TimeBeforeFall = 5.0f; // Default time before falling
	ResetDelay = 2.0f;     // Default delay before resetting platform to initial position
}

// Called when the game starts or when spawned
void AFallingPlateform::BeginPlay()
{
	Super::BeginPlay();
	numPlayersInside = 0;
	InitialLocation = GetActorLocation();
	
}

// Called every frame
void AFallingPlateform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCharacterOnPlatform)
	{
		TimeSinceCharacterOnPlatform += DeltaTime;
		if (TimeSinceCharacterOnPlatform >= TimeBeforeFall)
		{
			FallPlatform();
		}

	}
	if (bPlateformFall) 
	{
		TimePlateformFall += DeltaTime;
		if (TimePlateformFall >= ResetDelay)
		{
			ResetPlatform();
		}
		
	}
}

void AFallingPlateform::FallPlatform()
{
	// Simule la gravité pour faire tomber la plateforme
	PlatformMesh->SetSimulatePhysics(true);
	bPlateformFall = true;

}
void AFallingPlateform::ResetPlatform()
{
	// Remet la plateforme à sa position d'origine
	PlatformMesh->SetSimulatePhysics(false);
	SetActorLocation(InitialLocation);
	bPlateformFall = false;
	TimeSinceCharacterOnPlatform = 0.0f;
	TimePlateformFall = 0.0f;
}


void AFallingPlateform::OnCharacterOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,	const FHitResult& SweepResult)
{
	ADiamondProjectCharacter* Character = Cast<ADiamondProjectCharacter>(OtherActor);
	if (Character)
	{
		numPlayersInside++;
		if (!bCharacterOnPlatform)
		{
			bCharacterOnPlatform = true;
			TimeSinceCharacterOnPlatform = 0.0f;
		}
		
	}
}

void AFallingPlateform::OnCharacterOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ADiamondProjectCharacter* Character = Cast<ADiamondProjectCharacter>(OtherActor);
	if (Character)
	{
		numPlayersInside--;
		if (numPlayersInside <= 0)
		{
			bCharacterOnPlatform = false;
		}
	}
}


