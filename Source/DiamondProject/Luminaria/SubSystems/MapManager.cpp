#include "DiamondProject/Luminaria/SubSystems/MapManager.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"

#include "Engine/Texture.h"
#include "Engine/Texture2D.h"

#include "DiamondProject/Luminaria/UMG/MapWidget.h"

#include "Kismet/GameplayStatics.h"
#include "DiamondProject/Luminaria/Actors/CameraArea.h"

void UMapManager::Initialize(FSubsystemCollectionBase& Collection) {
	GEngine->AddOnScreenDebugMessage(-1, 100.F, FColor::Black, TEXT("Initialize Map Manager"));
	//CreateMap();
}

void UMapManager::CreateMap() {
	MapTextureSize = FVector2D(2048, 2048);

	MapTexture = UTexture2D::CreateTransient(MapTextureSize.X, MapTextureSize.Y); // Taille à gérer plus tard
	MapTexture->UpdateResource();

	FTexture2DMipMap& MipMap = MapTexture->PlatformData->Mips[0];
	FByteBulkData& ImageData = MipMap.BulkData;
	uint8* RawImageData = (uint8*)ImageData.Lock(LOCK_READ_WRITE);

	TArray<FColor> PixelData;
	PixelData.Init(FColor(255,255,255,0), MapTextureSize.X * MapTextureSize.Y);

	ACameraArea* FirstRoom = GetFirstRoom();

	if (!FirstRoom) {
		GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Red, TEXT("Room Not Detected. Map Can't Be Created"));
		return;
	}

	// Ref d'une salle normal : x = 1000;y  = 650 soit un ratio de 1,54
	// Ref d'une salle normal en pixel : x = 38; y = 25  (sur du 512x512)

	DrawRoom(PixelData, FVector2D(300 - 38, 1024 - 25), FVector2D(300 + 38, 1024 + 25), FVector2D(2048, 2048),FColor::Red);
	DetectOtherRooms(PixelData, FirstRoom, FVector2D(300 + 38, 1024 - 25));
	
	FMemory::Memcpy(RawImageData, PixelData.GetData(), PixelData.Num() * sizeof(FColor));

	ImageData.Unlock();
	MapTexture->UpdateResource();
}

TArray<uint16> UMapManager::DrawRoom(TArray<FColor>& PixelData, FVector2D Start, FVector2D End,FVector2D TexSize,FColor RoomColor) {
	TArray<uint16> RoomPixelData;

	for (int h = Start.Y; h < End.Y; h++) {
		for (int w = Start.X; w < End.X; w++) {
			if (h == Start.Y) {
				PixelData[w + (h - 1) * TexSize.X] = FColor::White;
				PixelData[w + (h - 2) * TexSize.X] = FColor::White;
			}
			if (h == End.Y - 1) {
				PixelData[w + (h + 1) * TexSize.X] = FColor::White;
				PixelData[w + (h + 2) * TexSize.X] = FColor::White;
			}

			if (w == Start.X) {
				PixelData[(w - 1) + h * TexSize.X] = FColor::White;
				PixelData[(w - 2) + h * TexSize.X] = FColor::White;
			}

			if (w == End.X - 1) {
				PixelData[(w + 1) + h * TexSize.X] = FColor::White;
				PixelData[(w + 2) + h * TexSize.X] = FColor::White;
			}

			PixelData[w + h * TexSize.X] = RoomColor;
			RoomPixelData.Push(w + h * TexSize.X);
		}
	}

	return RoomPixelData;
}

void UMapManager::DetectOtherRooms(TArray<FColor>& PixelData, ACameraArea* From, FVector2D EndFrom) {
	ACameraArea* LeftRoom = nullptr;
	ACameraArea* UpRoom = nullptr;
	ACameraArea* DownRoom = nullptr;

	check(GetWorld());

	FVector Start = From->GetActorLocation();
	FVector End = Start + FVector::RightVector * 50000.F;

	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params;

	Params.AddIgnoredActor(From);

	GetWorld()->LineTraceMultiByChannel(HitResults, Start, End, ECC_Visibility, Params);

	for (FHitResult Result : HitResults) {
		if (ACameraArea* Room = Cast<ACameraArea>(Result.GetActor())) {
			LeftRoom = Room;
			break;
		}
	}

	End = Start + FVector::UpVector * 50000.F;

	GetWorld()->LineTraceMultiByChannel(HitResults, Start, End, ECC_Visibility, Params);

	for (FHitResult Result : HitResults) {
		if (ACameraArea* Room = Cast<ACameraArea>(Result.GetActor())) {
			UpRoom = Room;
			break;
		}
	}

	End = Start + FVector::DownVector * 50000.F;

	GetWorld()->LineTraceMultiByChannel(HitResults, Start, End, ECC_Visibility, Params);

	for (FHitResult Result : HitResults) {
		if (ACameraArea* Room = Cast<ACameraArea>(Result.GetActor())) {
			DownRoom = Room;
			break;
		}
	}


	if (LeftRoom) {
		FVector2D StartDraw = EndFrom + FVector2D(2, 0);
		FVector2D EndDraw = StartDraw + FVector2D(38 * 2, 25 * 2);

		//DrawRoom(PixelData, StartDraw, EndDraw, MapTextureSize,FColor::Red);
		DetectOtherRooms(PixelData, LeftRoom, FVector2D(EndDraw.X,StartDraw.Y));
	}
	
	if (UpRoom) {
		FVector2D StartDraw = EndFrom - FVector2D(38 * 2, 25 * 2);
		FVector2D EndDraw = EndFrom;

		DrawRoom(PixelData, StartDraw, EndDraw, MapTextureSize, FColor::Yellow);
		//DetectOtherRooms(PixelData, LeftRoom, FVector2D(EndDraw.X, StartDraw.Y));
	}

	if (DownRoom) {
		FVector2D StartDraw = EndFrom + FVector2D(38 * 2, 25 * 2) + FVector2D(0,-2);
		FVector2D EndDraw = EndFrom + FVector2D(0, -2);
		GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Magenta, TEXT("Find Down Room"));
		DrawRoom(PixelData, StartDraw, EndDraw, MapTextureSize, FColor::Magenta);
		//DetectOtherRooms(PixelData, LeftRoom, FVector2D(EndDraw.X, StartDraw.Y));
	}
}

ACameraArea* UMapManager::GetFirstRoom() {
	TArray<AActor*> AllRooms;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraArea::StaticClass(), AllRooms);

	if (AllRooms.Num() == 0) {
		GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Red, TEXT("No Room Set.Please Set Some Room To Available Map"));
		return nullptr;
	}

	float MinY = AllRooms[0]->GetActorLocation().Y;
	AActor* MinRoom = AllRooms[0];

	for (AActor* Room : AllRooms) {
		FVector RoomPosition = Room->GetActorLocation();

		if (RoomPosition.Y < MinY) {
			MinY = RoomPosition.Y;
			MinRoom = Room;
		}
	}

	return Cast<ACameraArea>(MinRoom);
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
