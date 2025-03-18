#pragma once


class MathUtils
{
private:
	inline static const float DEG2RAD = 3.141593f / 180.0f;
	inline static const float RAD2DEG = 180.0f / 3.141593f;

public:
	inline float static radians(float degrees)
	{
		return degrees * DEG2RAD;
	}

	inline float static degrees(float radians)
	{
		return radians * RAD2DEG;
	}
};