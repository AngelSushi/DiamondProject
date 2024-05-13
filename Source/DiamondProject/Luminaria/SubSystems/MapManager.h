#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MapManager.generated.h"

class ACameraArea;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOpenMap,ADiamondProjectCharacter*,Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCloseMap, ADiamondProjectCharacter*, Character);
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

	UFUNCTION(BlueprintCallable,BlueprintPure)
	const TArray<ACameraArea*>& GetDrawnRooms() const { return DrawnRooms; }

private:
	UFUNCTION()
	void CreateMap();

	UFUNCTION()
	void DrawRoom(TArray<FColor>& PixelData,ACameraArea* From, FVector2D Start, FVector2D End,FVector2D TexSize,FColor RoomColor);
	
	UFUNCTION()
	void DetectOtherRooms(TArray<FColor>& PixelData, ACameraArea* From,FVector2D EndFrom);

	UFUNCTION()
	ACameraArea* Raycast(const ACameraArea* From,FVector Start,FVector End);
	
	UFUNCTION()
	ACameraArea* GetFirstRoom();

	UFUNCTION()
	FVector2D GetDrawSize(FVector RoomBoxSize);

	UPROPERTY()
	UTexture2D* MapTexture;

	UPROPERTY()
	TArray<ACameraArea*> DrawnRooms;

	FVector2D MapTextureSize;
};
