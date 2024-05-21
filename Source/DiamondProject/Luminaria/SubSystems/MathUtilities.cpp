#include "DiamondProject/Luminaria/SubSystems/MathUtilities.h"

float UMathUtilities::PingPong(float Value, float Min, float Max) {
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

	return Min + PingPongValue;
}
