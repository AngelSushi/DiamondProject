#include "DiamondProject/Luminaria/SubSystems/MapManager.h"
#include "DiamondProject/Luminaria/Core/DiamondProjectCharacter.h"

#include "Engine/Texture.h"
#include "Engine/Texture2D.h"

#include "DiamondProject/Luminaria/UMG/MapWidget.h"

#include "Kismet/GameplayStatics.h"
#include "DiamondProject/Luminaria/Actors/CameraArea.h"	
#include "Components/BoxComponent.h"

void UMapManager::Initialize(FSubsystemCollectionBase& Collection) {
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
	
	FVector2D DrawSize = GetDrawSize(FirstRoom->BoxCollision->GetUnscaledBoxExtent());

	DrawRoom(PixelData,FirstRoom, FVector2D(300 - DrawSize.X,1024 - DrawSize.Y), FVector2D(300 + DrawSize.X, 1024 + DrawSize.Y), MapTextureSize,FColor::Red);
	DetectOtherRooms(PixelData, FirstRoom, FVector2D(300 + DrawSize.X,1024 - DrawSize.Y));
	
	FMemory::Memcpy(RawImageData, PixelData.GetData(), PixelData.Num() * sizeof(FColor));

	ImageData.Unlock();
	MapTexture->UpdateResource();
}

void UMapManager::DetectOtherRooms(TArray<FColor>& PixelData, ACameraArea* From, FVector2D EndFrom) {
	ACameraArea* RightRoom = nullptr;
	ACameraArea* UpRoom = nullptr;
	ACameraArea* DownRoom = nullptr;

	check(GetWorld());

	FVector Start = From->GetActorLocation();

	float DistanceY = From->BoxCollision->GetScaledBoxExtent().Y + 50.F;
	float DistanceZ = From->BoxCollision->GetScaledBoxExtent().Z + 50.F;

	FVector End = Start + FVector::RightVector * DistanceY; // Pas mettre 50000 mais la distance de la zoomArea
	RightRoom = Raycast(From,Start, End);

	End = Start + FVector::UpVector * 50000.F;
	UpRoom = Raycast(From, Start, End);

	End = Start + FVector::DownVector * 50000.F;
	DownRoom = Raycast(From, Start, End);

	if (RightRoom && !DrawnRooms.Contains(RightRoom) && RightRoom->HasVisited()) { 
		FVector2D StartDraw = EndFrom + FVector2D(2, 0);
		FVector2D DrawSize = GetDrawSize(RightRoom->BoxCollision->GetUnscaledBoxExtent());
		FVector2D EndDraw = FVector2D((int)(StartDraw + DrawSize * 2).X, (int)(StartDraw + DrawSize * 2).Y);

		DrawRoom(PixelData,From,StartDraw, EndDraw, MapTextureSize,FColor::Red);
		DetectOtherRooms(PixelData, RightRoom, FVector2D(EndDraw.X,StartDraw.Y));
	}
	else if(!RightRoom) { // If there is no RightRoom Detect
		Start = Start + FVector::RightVector * DistanceY;
		End = Start + FVector::UpVector * DistanceZ;

		RightRoom = Raycast(From, Start, End);

		if (RightRoom && !DrawnRooms.Contains(RightRoom) && RightRoom->HasVisited()) { // We check if there is smaller area in right direction 
			FVector2D StartDraw = EndFrom + FVector2D(2, 0);
			FVector2D DrawSize = GetDrawSize(RightRoom->BoxCollision->GetScaledBoxExtent());
			FVector2D EndDraw = FVector2D((int) (StartDraw + DrawSize * 2).X,(int) (StartDraw + DrawSize * 2).Y);

			DrawRoom(PixelData,From,StartDraw, EndDraw, MapTextureSize,FColor::Yellow);
			DetectOtherRooms(PixelData, RightRoom, FVector2D(EndDraw.X, StartDraw.Y));
		}
	}
	
	if (UpRoom && !DrawnRooms.Contains(UpRoom) && UpRoom->HasVisited()) {
		FVector2D DrawSize = FVector2D((int)GetDrawSize(UpRoom->BoxCollision->GetScaledBoxExtent()).X,(int)GetDrawSize(UpRoom->BoxCollision->GetScaledBoxExtent()).Y);
		FVector2D StartDraw = EndFrom - FVector2D(DrawSize.X * 2,DrawSize.Y * 2) - FVector2D(0,2);
		FVector2D EndDraw = StartDraw + FVector2D(DrawSize.X * 2,DrawSize.Y * 2);

		DrawRoom(PixelData,From,StartDraw, EndDraw, MapTextureSize, FColor::Red);
		DetectOtherRooms(PixelData, UpRoom, FVector2D(EndDraw.X, StartDraw.Y));
	}

	if (DownRoom && !DrawnRooms.Contains(DownRoom) && DownRoom->HasVisited()) {
		FVector2D DrawSize = FVector2D((int)GetDrawSize(DownRoom->BoxCollision->GetScaledBoxExtent()).X, (int)GetDrawSize(DownRoom->BoxCollision->GetScaledBoxExtent()).Y);
		FVector2D StartDraw = EndFrom + FVector2D(-DrawSize.X * 2,DrawSize.Y * 2) + FVector2D(0,2);
		FVector2D EndDraw = StartDraw  + FVector2D(DrawSize.X * 2, DrawSize.Y * 2);

		DrawRoom(PixelData,From,StartDraw,EndDraw,MapTextureSize,FColor::Red);
		DetectOtherRooms(PixelData,DownRoom,FVector2D(EndDraw.X,StartDraw.Y));
	}
}


void UMapManager::DrawRoom(TArray<FColor>& PixelData, ACameraArea* From, FVector2D Start, FVector2D End, FVector2D TexSize, FColor RoomColor) {
	for (int h = Start.Y; h < End.Y; h++) {
		for (int w = Start.X; w < End.X; w++) {

			if (h == Start.Y && w == Start.X) {
				From->SetTopLeftCorner(FVector2D(w,h));
				// Ecrire les coords par rapport a la viewport et non a la texture  
			}

			if (h == Start.Y && w == (End.X - 1)) {
				From->SetTopRightCorner(FVector2D(w, h));
			}

			if (h == (End.Y - 1) && w == Start.X) {
				From->SetBotLeftCorner(FVector2D(w, h));
			}

			if (h == (End.Y - 1) && w == (End.X - 1)) {
				From->SetBotRightCorner(FVector2D(w, h));
			}

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
			//PixelsDrawn.Push(FVector2D(w / TexSize.X,h / TexSize.Y));
		}
	}
	DrawnRooms.Add(From);

	// AJoutez unevariable dans cameraarea pour avoir la liste des pixels
}

ACameraArea* UMapManager::Raycast(const ACameraArea* From,FVector Start, FVector End) {
	TArray<FHitResult> HitResults;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(From);

	GetWorld()->LineTraceMultiByChannel(HitResults, Start, End, ECC_Visibility,Params);

	for (FHitResult Result : HitResults) {
		if (ACameraArea* Room = Cast<ACameraArea>(Result.GetActor())) {
			return Room;
		}
	}

	return nullptr;
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

FVector2D UMapManager::GetDrawSize(FVector RoomBoxSize) {
	FVector RefRoomSize = FVector(50, 1000, 650);
	FVector RefDrawSize = FVector(0, 38, 25);
	FVector TargetRoomSize = RoomBoxSize;

	FVector TargetRef = TargetRoomSize / RefRoomSize;
	FVector TargetDrawSize = TargetRef * RefDrawSize;

	return FVector2D(TargetDrawSize.Y, TargetDrawSize.Z);
}

void UMapManager::OpenMap(ADiamondProjectCharacter* Character) {
	if (Character->MapWidget) {
		CreateMap();
		Character->MapWidget->MapImage->SetBrushFromTexture(MapTexture,true);
	
		OnOpenMap.Broadcast(Character);
	}
}

void UMapManager::CloseMap(ADiamondProjectCharacter* Character) {
	if (Character->MapWidget) {
		Character->MapWidget->RemoveFromViewport();

		OnCloseMap.Broadcast(Character);
	}
}
