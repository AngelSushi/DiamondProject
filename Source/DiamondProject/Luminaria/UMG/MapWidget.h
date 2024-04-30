#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "MapWidget.generated.h"

UCLASS()
class DIAMONDPROJECT_API UMapWidget : public UUserWidget {
	GENERATED_BODY()

public:	

	UPROPERTY(BlueprintReadWrite,meta =(BindWidget))
	TObjectPtr<UImage> MapImage;
	
};
