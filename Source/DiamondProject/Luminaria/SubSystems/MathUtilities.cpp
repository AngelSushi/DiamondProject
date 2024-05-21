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

FVector UMathUtilities::PingPongVec(float Value, FVector& Min, FVector& Max) {
	float X = PingPongFloat(Value, Min.X, Max.X);
	float Y = PingPongFloat(Value, Min.Y, Max.Y);
	float Z = PingPongFloat(Value, Min.Z, Max.Z);

	return FVector(X, Y, Z);
}

FVector UMathUtilities::PingPongVecAscending(float Value, FVector& Min, FVector& Max, bool& IsPing) {
	float X = PingPongFloatAscending(Value, Min.X, Max.X,IsPing);
	float Y = PingPongFloatAscending(Value, Min.Y, Max.Y,IsPing);
	float Z = PingPongFloatAscending(Value, Min.Z, Max.Z,IsPing);

	return FVector(X, Y, Z);
}
