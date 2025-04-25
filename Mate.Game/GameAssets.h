#pragma once
#include <ECS.h>
#include <Mate.h>

class GameAssets
{
public:
	static TransformComponent& CreatePlayer(ECS& ecs, ModelImporter& modelImporter, Vector3 basePos = Vector3());
	static void CreateReward(ECS& ecs, ModelImporter& modelImporter, Vector3 basePos = Vector3());
	static void CreateObstacle(ECS& ecs, ModelImporter& modelImporter, Vector3 basePos = Vector3());
};

