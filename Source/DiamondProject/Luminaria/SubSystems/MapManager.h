#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MapManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOpenMap,ADiamondProjectCharacter*,Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCloseMap, ADiamondProjectCharacter*, Character);

class ACameraArea;

UCLASS()
class DIAMONDPROJECT_API UMapManager : public UWorldSubsystem {
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UPROPERTY(VisibleAnywhere)
	FOnOpenMap OnOpenMap;

	UPROPERTY(VisibleAnywhere)
	FOnCloseMap OnCloseMap;

	UFUNCTION()
	void OpenMap(ADiamondProjectCharacter* Character);

	UFUNCTION()
	void CloseMap(ADiamondProjectCharacter* Character);

private:
	UFUNCTION()
	void CreateMap();

	UFUNCTION()
	TArray<uint16> DrawRoom(TArray<FColor>& PixelData, FVector2D Start, FVector2D End,FVector2D TexSize,FColor RoomColor);
	
	UFUNCTION()
	void DetectOtherRooms(TArray<FColor>& PixelData, ACameraArea* From,FVector2D EndFrom);

	ACameraArea* GetFirstRoom();

	UPROPERTY()
	UTexture2D* MapTexture;

	FVector2D MapTextureSize;
};
