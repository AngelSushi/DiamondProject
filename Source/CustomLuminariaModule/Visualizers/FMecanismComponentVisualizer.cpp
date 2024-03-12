#include "FMecanismComponentVisualizer.h"
#include "DiamondProject/Luminaria/ActorComponents/MecanismComponent.h"
#include "../Proxies/HMecanismProxy.h"
#include "CustomLuminariaModule/CustomLuminariaModule.h"
#include "Kismet/KismetSystemLibrary.h"
#include "InputCoreTypes.h"
#include "InputCore.h"


FMecanismComponentVisualizer::FMecanismComponentVisualizer() {
}

FMecanismComponentVisualizer::~FMecanismComponentVisualizer() {
}

void FMecanismComponentVisualizer::OnRegister() {
	UE_LOG(CustomLuminariaModule, Warning, TEXT("Register Mecanism Visualizer"));
	CurrentViewport = nullptr;
}

void FMecanismComponentVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) {
	if (const UMecanismComponent* MecanismComponent = Cast<UMecanismComponent>(Component)) {

		//UE_LOG(CustomLuminariaModule, Log, TEXT("Draw Visu here"));

		FVector BoxOrigin, BoxExtent;
		float SphereRadius;

		UKismetSystemLibrary::GetComponentBounds(MecanismComponent, BoxOrigin, BoxExtent, SphereRadius);
		FVector Location = MecanismComponent->GetComponentLocation() + FVector(0, 0,BoxOrigin.Z + BoxExtent.Z) + FVector::UpVector * 10;

		PDI->SetHitProxy(new HMecanismProxy(Component,0));
		DrawWireSphere(PDI, Location, FColor::Red, 25, 16, SDPG_Foreground);
		PDI->SetHitProxy(nullptr);


	}
}

void FMecanismComponentVisualizer::DrawWireSphere(FPrimitiveDrawInterface* PDI, const FVector& Center, const FColor& Color, float Radius, int32 NumSides, uint8 DepthPriorityGroup) const {
	const float DeltaPhi = PI / 16;
	const float DeltaTheta = 2 * PI / NumSides;

	for (int32 RingIndex = 0; RingIndex <= 16; ++RingIndex) {
		const float Phi = DeltaPhi * RingIndex;
		const float SinPhi = FMath::Sin(Phi);
		const float CosPhi = FMath::Cos(Phi);

		for (int32 SideIndex = 0; SideIndex < NumSides; ++SideIndex) {
			const float Theta = DeltaTheta * SideIndex;
			const FVector ThisVertex = Center + FVector(Radius * FMath::Cos(Theta) * SinPhi, Radius * FMath::Sin(Theta) * SinPhi, Radius * CosPhi);
			const FVector NextVertex = Center + FVector(Radius * FMath::Cos(Theta + DeltaTheta) * SinPhi, Radius * FMath::Sin(Theta + DeltaTheta) * SinPhi, Radius * CosPhi);
			PDI->DrawLine(ThisVertex, NextVertex, Color, DepthPriorityGroup);
		}
	}

	for (int32 SideIndex = 0; SideIndex < NumSides; ++SideIndex) {
		const float Theta = DeltaTheta * SideIndex;

		for (int32 RingIndex = 0; RingIndex < 16; ++RingIndex) {
			const float Phi = DeltaPhi * RingIndex;
			const float NextPhi = Phi + DeltaPhi;
			const float SinPhi = FMath::Sin(Phi);
			const float CosPhi = FMath::Cos(Phi);
			const float NextSinPhi = FMath::Sin(NextPhi);
			const float NextCosPhi = FMath::Cos(NextPhi);

			const FVector ThisVertex = Center + FVector(Radius * FMath::Cos(Theta) * SinPhi, Radius * FMath::Sin(Theta) * SinPhi, Radius * CosPhi);
			const FVector NextVertex = Center + FVector(Radius * FMath::Cos(Theta) * NextSinPhi, Radius * FMath::Sin(Theta) * NextSinPhi, Radius * NextCosPhi);
			PDI->DrawLine(ThisVertex, NextVertex, Color, DepthPriorityGroup);
		}
	}
}

bool FMecanismComponentVisualizer::VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click) {
	UE_LOG(CustomLuminariaModule, Log, TEXT("Hit Here On Vis Proxy"));
	CurrentViewport = InViewportClient;
	return false;
}

void FMecanismComponentVisualizer::EndEditing() {

	UE_LOG(CustomLuminariaModule, Log, TEXT("End"));
}

bool FMecanismComponentVisualizer::GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const {
	return false;
}

bool FMecanismComponentVisualizer::GetCustomInputCoordinateSystem(const FEditorViewportClient* ViewportClient, FMatrix& OutMatrix) const {
	return false;
}

bool FMecanismComponentVisualizer::HandleInputDelta(FEditorViewportClient* ViewportClient, FViewport* Viewport, FVector& DeltaTranslate, FRotator& DeltaRotate, FVector& DeltaScale) {
	UE_LOG(CustomLuminariaModule, Log, TEXT("Hit Pending Input Delta"));
	
	return true;
}

bool FMecanismComponentVisualizer::HandleInputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event) {
	UE_LOG(CustomLuminariaModule, Log, TEXT("Handle Input Key")); 
	return true;
}

TSharedPtr<SWidget> FMecanismComponentVisualizer::GenerateContextMenu() const {
	return TSharedPtr<SWidget>();
}

UMecanismComponent* FMecanismComponentVisualizer::GetEditedTargetingComponent() const {
	return nullptr;
}

void FMecanismComponentVisualizer::DrawVisualizationHUD(const UActorComponent* Component, const FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) {
	/*UE_LOG(CustomLuminariaModule, Log, TEXT("My Log"));

	if (CurrentViewport) {
		if (Viewport->KeyState(EKeys::LeftMouseButton)) {
			UE_LOG(CustomLuminariaModule, Log, TEXT("Click Handle"));
			CurrentViewport->bDisableInput = true;
			CurrentViewport->Cursor
		}
		else {
			if (CurrentViewport) {
				CurrentViewport->bDisableInput = false;
			}
		}
	}*/
}

bool FMecanismComponentVisualizer::IsVisualizingArchetype() {
	return false;
}

