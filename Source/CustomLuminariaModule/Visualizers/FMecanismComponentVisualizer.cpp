#include "FMecanismComponentVisualizer.h"

FMecanismComponentVisualizer::FMecanismComponentVisualizer() {
}

FMecanismComponentVisualizer::~FMecanismComponentVisualizer() {
}

void FMecanismComponentVisualizer::OnRegister() {
}

void FMecanismComponentVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) {
}

bool FMecanismComponentVisualizer::VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click) {
	return false;
}

void FMecanismComponentVisualizer::EndEditing() {
}

bool FMecanismComponentVisualizer::GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const {
	return false;
}

bool FMecanismComponentVisualizer::GetCustomInputCoordinateSystem(const FEditorViewportClient* ViewportClient, FMatrix& OutMatrix) const {
	return false;
}

bool FMecanismComponentVisualizer::HandleInputDelta(FEditorViewportClient* ViewportClient, FViewport* Viewport, FVector& DeltaTranslate, FRotator& DeltaRotate, FVector& DeltaScale) {
	return false;
}

bool FMecanismComponentVisualizer::HandleInputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event) {
	return false;
}

TSharedPtr<SWidget> FMecanismComponentVisualizer::GenerateContextMenu() const {
	return TSharedPtr<SWidget>();
}

UMecanismComponent* FMecanismComponentVisualizer::GetEditedTargetingComponent() const {
	return nullptr;
}

void FMecanismComponentVisualizer::DrawVisualizationHUD(const UActorComponent* Component, const FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) {
}

bool FMecanismComponentVisualizer::IsVisualizingArchetype() {
	return false;
}

