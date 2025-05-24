#pragma once
#include "EnvironmentAssets.h"
#include <deque>

class EndlessRunner
{
public:
	EndlessRunner(Engine* engine);

	void Update(float deltaTime);
	void FixedUpdate();
	void ProcessPlayerInput();

	void MovePlayer(float fixedDeltaTime);
	void MoveEnvironment(float fixedDeltaTime);
	
	bool IsGameRunning();
	void SetGameRunning(bool status);
	static constexpr float fixedDeltaTime = 1.0f / 60.0f;

private:
	Engine* engine;

	std::unique_ptr<Entity> player;
	std::deque<EnvironmentPart> environmentParts;

	Vector3 playerStartPos;
	PlayerRailState railState;

	bool runGame = false;
	float accumulator = 0.0f;
	int points = 0;

	const std::string REWARD_TAG = "COIN";
	const std::string OBSTACLE_TAG = "BOX";
	const int ROAD_LENGHT = 3;
	const int START_COINS = 10;
	const int START_OBSTACLES = 3;
	const float playerRailSpace = 1.5f;
	
	void Setup();
	void CreateEnvironment();

	EnvironmentPart CreateEnvironmentPart(int i);
	void RedoEnvironmentPart(EnvironmentPart& part, bool init = false);
	void ResetEnvironmentPart(EnvironmentPart& part, const Vector3& newFloorPos);

	Part CreateObstacle(Vector3 pos);
	Part CreateReward(Vector3 pos);
};