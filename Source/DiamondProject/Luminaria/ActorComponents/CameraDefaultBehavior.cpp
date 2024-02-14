#include "DiamondProject/Luminaria/ActorComponents/CameraDefaultBehavior.h"
#include "DiamondProject/Luminaria/Actors/LuminariaCamera.h"
#include "DiamondProject/Luminaria/SubSystems/PlayerEventsDispatcher.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"

UCameraDefaultBehavior::UCameraDefaultBehavior()
{
}

void UCameraDefaultBehavior::BeginPlay()
{
	Super::BeginPlay();

	UPlayerEventsDispatcher* PlayerEventsDispatcher = GetWorld()->GetSubsystem<UPlayerEventsDispatcher>();
	PlayerEventsDispatcher->OnPlayerRegister.AddDynamic(this,&UCameraDefaultBehavior::RegisterPlayer);
	PlayerEventsDispatcher->OnPlayerMove.AddDynamic(this,&UCameraDefaultBehavior::OnPlayerMove);

	_defaultY = OwnerActor->GetActorLocation().Y;
}

void UCameraDefaultBehavior::RegisterPlayer(ADiamondProjectCharacter* Character)
{
	_characters.AddUnique(Character);

	bool isCanceled = false;
	OnPlayerMove(Character,FVector2D(),isCanceled);
}

void UCameraDefaultBehavior::OnPlayerMove(ADiamondProjectCharacter* character, FVector2D direction, bool& isCanceled)
{
	if(_characters.Num() >= 2)
	{
		for(ADiamondProjectCharacter* player : _characters) {
			if(const APlayerController* PlayerController = Cast<APlayerController>(player->GetController())) {
				FVector2D WorldScreenPos = FVector2D();
				PlayerController->ProjectWorldLocationToScreen(player->GetActorLocation(),WorldScreenPos);

				FVector2D ViewportSize = FVector2D();
			
				GEngine->GameViewport->GetViewportSize(ViewportSize);

				if(WorldScreenPos.X < 0 || WorldScreenPos.X > ViewportSize.X)
				{
					isCanceled = true;
					return;
				}
			}
		}
		
		
	}
}

void UCameraDefaultBehavior::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (_characters.Num() >= 2) {
		FVector First = _characters[0]->GetActorLocation();
		FVector Second = _characters[1]->GetActorLocation();

		float divider = 2.F;

		if (!_characters[0]->GetMesh()->IsVisible()) {
			First = FVector::Zero();
			divider -= 1.F;
		}

		if (!_characters[1]->GetMesh()->IsVisible()) {
			Second = FVector::Zero();
			divider -= 1.F;
		}

		FVector barycenter = (First + Second) / divider;
		barycenter += FVector(0, 0, 45.F);
		barycenter.Y = _defaultY;

		OwnerActor->SetActorLocation(barycenter);
	}
}








