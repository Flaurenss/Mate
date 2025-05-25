#pragma once

#include <deque>
#include "EnvironmentAssets.h"

class EndlessRunner
{
public:
	EndlessRunner(Engine* engine);

	void Run();

private:
	static constexpr float fixedDeltaTime = 1.0f / 60.0f;
	Engine* engine;

	std::unique_ptr<Entity> player;
	std::deque<EnvironmentPart> environmentParts;

	Vector3 playerStartPos;
	PlayerRailState railState;

	bool runGame = false;
	float accumulator = 0.0f;
	int points = 0;
	float environmentSpeed = 7.5f;
	float maxEnvironmentSpeed = 20.0f;

	const std::string REWARD_TAG = "COIN";
	const std::string OBSTACLE_TAG = "BOX";
	const int ROAD_LENGHT = 1;
	const int START_COINS = 6;
	const int START_OBSTACLES = 3;
	const float playerRailSpace = 1.5f;

	void Setup();
	void Update(float deltaTime);
	void FixedUpdate();
	Entity CreateCamera();
	void ProcessPlayerInput();
	void MovePlayer(float fixedDeltaTime);

	void CreateEnvironment();
	EnvironmentPart CreateEnvironmentPart(int i);
	Part CreateObstacle(Vector3 pos);
	Part CreateReward(Vector3 pos);

	void ResetEnvironmentPart(EnvironmentPart& part, const Vector3& newFloorPos);
	void RedoEnvironmentPart(EnvironmentPart& part, bool init = false);

	void MoveEnvironment(float fixedDeltaTime);

	/// <summary>
	/// Increase difficulty every X coins picked.
	/// </summary>
	void IncreaseDifficulty();
};