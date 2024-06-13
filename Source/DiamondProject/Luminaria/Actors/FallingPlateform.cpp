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

	bIsShaking = false;
	ShakeDuration = 2.0f;
	ShakeTime = 0.0f;
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
	
	if (bIsShaking)
	{
		ShakeTime += DeltaTime;
		if (ShakeTime >= ShakeDuration)
		{
			SetActorLocation(InitialLocation);
			bIsShaking = false;
		}
		else
		{
			ShakeBox();
		}
	}

	if (bCharacterOnPlatform)
	{
		if (!bPlateformAlreadyFall)
		{
			TimeSinceCharacterOnPlatform += DeltaTime;
			if (TimeSinceCharacterOnPlatform >= TimeBeforeFall)
			{
				FallPlatform();
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, FString::Printf(TEXT("Bool: %s"), bPlateformAlreadyFall ? TEXT("true") : TEXT("false")));
								
			}
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

void AFallingPlateform::ShakeBox()
{
	FVector NewLocation = InitialLocation;
	//NewLocation.X += FMath::RandRange(-5.0f, 5.0f);
	NewLocation.Y += FMath::RandRange(-5.0f, 5.0f);
	SetActorLocation(NewLocation);
}

void AFallingPlateform::FallPlatform()
{
	// Simule la gravité pour faire tomber la plateforme
	PlatformMesh->SetSimulatePhysics(true);
	DetectionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bPlateformFall = true;
	bPlateformAlreadyFall = true;

}
void AFallingPlateform::ResetPlatform()
{
	// Remet la plateforme à sa position d'origine
	PlatformMesh->SetSimulatePhysics(false);
	DetectionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetActorLocation(InitialLocation);
	bPlateformFall = false;
	TimeSinceCharacterOnPlatform = 0.0f;
	TimePlateformFall = 0.0f;
	ShakeTime = 0.0f;
	bPlateformAlreadyFall = false;

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
			bIsShaking = true;
			TimeSinceCharacterOnPlatform = 0.0f;
			ShakeTime = 0.0f;
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
			bIsShaking = false;

		}
	}
}


