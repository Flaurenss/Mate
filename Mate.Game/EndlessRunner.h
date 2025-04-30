#pragma once
#include "EnvironmentAssets.h"

class EndlessRunner
{
public:
	EndlessRunner(ECS& ecs);
	
	void Setup();

	void ProcessPlayerInput();

	std::vector<Vector3> GenerateBoxPoses();
	std::vector<Vector3> GenerateCoinPoses();
	void SetAssetsPoses(bool init);
private:
	ECS& ecs;

	ModelImporter modelImporter;
	Entity player;

	bool runGame = false;

	const std::string REWARD_TAG = "COIN";
	const std::string OBSTACLE_TAG = "BOX";
	const int ROAD_LENGHT = 3;
	const int START_COINS = 10;
	const int START_OBSTACLES = 3;
	const float fixedDeltaTime = 1.0f / 60.0f;
};