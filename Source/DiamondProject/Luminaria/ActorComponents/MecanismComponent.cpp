#include "MecanismComponent.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UMecanismComponent::UMecanismComponent() {
}



void UMecanismComponent::GenerateMesh(FVector Location) {
	float radius = 10.F;
	int32 numSegments = 32;
	int32 numStacks = 16;

	TArray<FVector> vertices;
	TArray<int32> triangles;
	TArray<FVector> normals;
	TArray<FVector> uvs;
	TArray<FLinearColor> vertexColors;
	TArray<FProcMeshTangent> tangents;


	FVector BoxExtent, BoxOrigin;
	float SphereRadius;

	UKismetSystemLibrary::GetComponentBounds(this, BoxOrigin, BoxExtent, SphereRadius);
	FVector ActorPosition = FVector(0, 0, BoxExtent.Z) + FVector::UpVector * (SphereRadius / 2 + 35.F);

	if (Location != FVector::Zero()) {
		ActorPosition = MeshLocation;
	}

	for (int stack = 0; stack <= numStacks; stack++) {
		for (int segment = 0; segment <= numSegments; segment++) {
			float theta = static_cast<float>(stack) / static_cast<float>(numStacks) * PI;
			float phi = static_cast<float>(segment) / static_cast<float>(numSegments) * 2 * PI;

			float x = radius * FMath::Sin(theta) * FMath::Cos(phi);
			float y = radius * FMath::Sin(theta) * FMath::Sin(phi);
			float z = radius * FMath::Cos(theta);

			vertices.Add(FVector(ActorPosition.X + x, ActorPosition.Y + y, ActorPosition.Z + z));
		}
	}

	for (int stack = 0; stack <= numStacks; stack++) {
		for (int segment = 0; segment <= numSegments; segment++) {

			int nextSegment = (segment + 1) % numSegments;
			int currentIndex = stack * (numSegments + 1) + segment;
			int nextIndex = currentIndex + 1;
			int nextStackIndex = (stack + 1) * (numSegments + 1) + segment;
			int nextStackNextIndex = nextStackIndex + 1;

			triangles.Add(currentIndex);
			triangles.Add(nextIndex);
			triangles.Add(nextStackIndex);

			triangles.Add(nextStackIndex);
			triangles.Add(nextIndex);
			triangles.Add(nextStackNextIndex);
		}
	}

	//proceduralMesh->CreateMeshSection_LinearColor(0, vertices, triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
	MeshLocation = ActorPosition;
}
