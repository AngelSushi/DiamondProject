#include "DiamondProjectGameMode.h"
#include "DiamondProjectPlayerController.h"
#include "DiamondProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

ADiamondProjectGameMode::ADiamondProjectGameMode() {
	PlayerControllerClass = ADiamondProjectPlayerController::StaticClass();

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr) {
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL) {
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}

void ADiamondProjectGameMode::BeginPlay() {
	Super::BeginPlay();

	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
	
	if (PlayerStarts.Num() < 2) {
		return;
	}
	
	//CreatePlayer(1,PlayerStarts[0]);
	//CreatePlayer(2, PlayerStarts[1]);
}

void ADiamondProjectGameMode::CreatePlayer(int ControllerId,AActor* PlayerStart) {
	GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Cyan, TEXT("Create My Player"));
	UGameInstance* GameInstance = GetGameInstance();

	if (!GameInstance) {
		GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Red, TEXT("Game Instance Not Be Found"));
		return;
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Yellow, FString::FromInt(GetNumPlayers()));

	FString OutError;
	ULocalPlayer* LocalPlayer = GameInstance->CreateLocalPlayer(ControllerId,OutError,true);

	if (!LocalPlayer) {
		FString Log = FString(TEXT("Log Error : ")) + OutError;
		GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Red, TEXT("FATAL ERROR : Creation Local Player Failed"));
		GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Red, Log);
		return;
	}

	LocalPlayer->SetControllerId(ControllerId);
	ACharacter* Player = GetWorld()->SpawnActor<ACharacter>(Character, PlayerStart->GetTransform());

	LocalPlayer->GetPlayerController(GetWorld())->Possess(Player);

	//APlayerController* PlayerController = GetWorld()->SpawnActor<APlayerController>(PlayerControllerClass, PlayerStart->GetTransform());
	//PlayerController->SetPlayer
	/*if (PlayerController) {
		
		ACharacter* Player = GetWorld()->SpawnActor<ACharacter>(Character,PlayerStart->GetTransform());
		
		PlayerController->Possess(Player);
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Red, TEXT("No Controller Spawned"));
	}

	*/
}
