#pragma once

#include "CoreMinimal.h"
#include "Componentvisualizer.h"
//#include "MecanismComponent.h"

class UMecanismComponent;

class CUSTOMLUMINARIAMODULE_API FMecanismComponentVisualizer : public FComponentVisualizer {


public:
	FMecanismComponentVisualizer();
	~FMecanismComponentVisualizer();

    virtual void OnRegister() override;
    virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
    virtual bool VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click) override;
    virtual void EndEditing() override;
    virtual bool GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const override;
    virtual bool GetCustomInputCoordinateSystem(const FEditorViewportClient* ViewportClient, FMatrix& OutMatrix) const override;
    virtual bool HandleInputDelta(FEditorViewportClient* ViewportClient, FViewport* Viewport, FVector& DeltaTranslate, FRotator& DeltaRotate, FVector& DeltaScale) override;
    virtual bool HandleInputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event) override;
    virtual TSharedPtr<SWidget> GenerateContextMenu() const override;

    UMecanismComponent* GetEditedTargetingComponent() const;

    virtual void DrawVisualizationHUD(const UActorComponent* Component, const FViewport* Viewport, const FSceneView* View, FCanvas* Canvas);
    virtual bool IsVisualizingArchetype();
    
    
private:
    void DrawWireSphere(FPrimitiveDrawInterface* PDI, const FVector& Center, const FColor& Color, float Radius, int32 NumSides, uint8 DepthPriorityGroup) const;

    UPROPERTY()
    FEditorViewportClient* CurrentViewport;
};
