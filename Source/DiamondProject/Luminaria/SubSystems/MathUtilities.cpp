#include "DiamondProject/Luminaria/SubSystems/MathUtilities.h"

float UMathUtilities::PingPongFloat(float Value, float Min, float Max) {
	if (Max <= Min) {
		return Min;
	}

	float Range = Max - Min;
	float TwiceRange = Range * 2;

	float ModValue = FMath::Fmod(Value - Min, TwiceRange);

	if (ModValue < 0) {
		ModValue += TwiceRange;
	}

	float PingPongValue = (ModValue < Range) ? ModValue : TwiceRange - ModValue;
	
	bool Pong = ModValue < Range;

	return Min + PingPongValue;
}

float UMathUtilities::PingPongFloatAscending(float Value, float Min, float Max, bool& IsPing) {
	if (Max <= Min) {
		IsPing = true;
		return Min;
	}

	float Range = Max - Min;
	float TwiceRange = Range * 2;

	float ModValue = FMath::Fmod(Value - Min, TwiceRange);

	if (ModValue < 0) {
		ModValue += TwiceRange;
	}

	float PingPongValue = (ModValue < Range) ? ModValue : TwiceRange - ModValue;

	IsPing = !(ModValue < Range);

	return Min + PingPongValue;
}

FVector UMathUtilities::PingPongVec(float Value, FVector Min, FVector Max) {

	//GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Yellow, FString::Printf(TEXT("Min %s"), *Min.ToString()));
	//GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Yellow, FString::Printf(TEXT("Max %s"), *Max.ToString()));

	if (Min.X >= Max.X && Min.Y >= Max.Y && Min.Z >= Max.Z) {
		//GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Blue, TEXT("Min Is Greater"));
		FVector SaveMax = Max;
		Max = Min;
		Min = SaveMax;
	}

	float X = PingPongFloat(Value, Min.X, Max.X);
	float Y = PingPongFloat(Value, Min.Y, Max.Y);
	float Z = PingPongFloat(Value, Min.Z, Max.Z);

	return FVector(X, Y, Z);
}

FVector UMathUtilities::PingPongVecAscending(float Value, FVector Min, FVector Max, bool& IsPing) {

	if (Min.X >= Max.X && Min.Y >= Max.Y && Min.Z >= Max.Z) {
		//GEngine->AddOnScreenDebugMessage(-1, 1.F, FColor::Blue, TEXT("Min Is Greater"));
		FVector SaveMax = Max;
		Max = Min;
		Min = SaveMax;
	}

	float X = PingPongFloatAscending(Value, Min.X, Max.X,IsPing);
	float Y = PingPongFloatAscending(Value, Min.Y, Max.Y,IsPing);
	float Z = PingPongFloatAscending(Value, Min.Z, Max.Z,IsPing);

	return FVector(X, Y, Z);
}
