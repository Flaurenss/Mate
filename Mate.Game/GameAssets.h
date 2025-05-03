#pragma once
#include <ECS.h>
#include <Mate.h>

class GameAssets
{
public:
	static Entity CreatePlayer(ECS& ecs, Vector3 basePos = Vector3(), Vector3 baseRot = Vector3(), Vector3 baseScale = Vector3::One);
	static Entity CreateReward(ECS& ecs, Vector3 basePos = Vector3(), Vector3 baseRot = Vector3(), Vector3 baseScale = Vector3::One);
	static Entity CreateObstacle(ECS& ecs, Vector3 basePos = Vector3(), Vector3 baseRot = Vector3(), Vector3 baseScale = Vector3::One);
};

