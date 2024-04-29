#include "DiamondProject/Luminaria/SubSystems/MapManager.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"

#include "Engine/Texture.h"
#include "Engine/Texture2D.h"

#include "DiamondProject/Luminaria/UMG/MapWidget.h"

void UMapManager::Initialize(FSubsystemCollectionBase& Collection) {
	GEngine->AddOnScreenDebugMessage(-1, 100.F, FColor::Black, TEXT("Initialize Map Manager"));
	//CreateMap();
}

void UMapManager::CreateMap() {
	MapTexture = UTexture2D::CreateTransient(2048, 2048); // Taille à gérer plus tard
	MapTexture->UpdateResource();

	FTexture2DMipMap& MipMap = MapTexture->PlatformData->Mips[0];
	FByteBulkData& ImageData = MipMap.BulkData;
	uint8* RawImageData = (uint8*)ImageData.Lock(LOCK_READ_WRITE);

	TArray<FColor> PixelData;
	PixelData.Init(FColor(255,255,255,255), 2048 * 2048);

	// Ref d'une salle normal : x = 1000;y  = 650 soit un ratio de 1,5
	// Ref d'une salle normal en pixel : x = 38; y = 25  (sur du 512x512)
	
	// Dessinez les contours pour voir si ca se repete 

	CreateRoom(PixelData, FVector2D(1024 - (25), 1024 - (38)), FVector2D(1024 + (25), 1024 + (38)));
	//CreateRoom(PixelData, FVector2D(256 - 25,256 - 38), FVector2D(256 + 25,256 + 38));

	FMemory::Memcpy(RawImageData, PixelData.GetData(), PixelData.Num() * sizeof(FColor));

	ImageData.Unlock();
	MapTexture->UpdateResource();
}

void UMapManager::CreateRoom(TArray<FColor>& PixelData, FVector2D Start, FVector2D End) {
	for (int h = Start.X; h < End.X; h++) {
		for (int w = Start.Y; w < End.Y; w++) {
			if (h == Start.X) {
				PixelData[w + (h - 1) * 512] = FColor::White;
				PixelData[w + (h - 2) * 512] = FColor::White;
			}
			if (h == End.X - 1) {
				PixelData[w + (h + 1) * 512] = FColor::White;
				PixelData[w + (h + 2) * 512] = FColor::White;
			}

			if (w == Start.Y) {
				PixelData[(w - 1) + h * 512] = FColor::White;
				PixelData[(w - 2) + h * 512] = FColor::White;
			}

			if (w == End.Y - 1) {
				PixelData[(w + 1) + h * 512] = FColor::White;
				PixelData[(w + 2) + h * 512] = FColor::White;
			}

			PixelData[w + h * 512] = FColor::Red;
		}
	}

}

void UMapManager::OpenMap(ADiamondProjectCharacter* Character) {
	if (Character->MapWidget) {
		CreateMap();
		GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Blue, TEXT("Map Func"));
		Character->MapWidget->MapImage->SetBrushFromTexture(MapTexture,true);
	
		OnOpenMap.Broadcast(Character);
	}
}

void UMapManager::CloseMap(ADiamondProjectCharacter* Character) {
}
