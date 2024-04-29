#include "DiamondProject/Luminaria/SubSystems/MapManager.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"

void UMapManager::Initialize(FSubsystemCollectionBase& Collection) {
	GEngine->AddOnScreenDebugMessage(-1, 100.F, FColor::Black, TEXT("Initialize Map Manager"));

	CreateMap();
}

void UMapManager::CreateMap() {
	/*MapTexture = UTexture2D::CreateTransient(100, 100); // Taille a gérer plus tard
	FTexturePlatformData** PlatformData = MapTexture->GetRunningPlatformData();
	FTexture2DMipMap FirstMip = (*PlatformData)->Mips[0];
	FByteBulkData ImageData = FirstMip.BulkData;
	uint8* RawImageData = (uint8*)ImageData.Lock(LOCK_READ_WRITE);

	int ArraySize = 100 * 100 * 4;

	for (int i = 0; i < ArraySize; i++) {
		RawImageData[i] = 255;
	}

	ImageData.Unlock();
	MapTexture->UpdateResource();
	*/
}

void UMapManager::OnOpenMapFunc(ADiamondProjectCharacter* Character) {
}

void UMapManager::OnCloseMapFunc(ADiamondProjectCharacter* Character) {
}
