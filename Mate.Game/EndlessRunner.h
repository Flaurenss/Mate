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

};