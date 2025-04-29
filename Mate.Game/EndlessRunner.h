#pragma once
#include "EnvironmentAssets.h"

class EndlessRunner
{
public:
	void ProcessPlayerInput();

	std::vector<Vector3> GenerateBoxPoses();
	std::vector<Vector3> GenerateCoinPoses();
	void SetAssetsPoses(bool init);
private:
	float fixedDeltaTime = 1.0f / 60.0f;
};