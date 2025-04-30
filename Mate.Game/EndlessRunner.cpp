#include "EndlessRunner.h"
#include "GameAssets.h"

EndlessRunner::EndlessRunner(ECS& ecs) :
	ecs(ecs),
	modelImporter(ModelImporter()),
	player(GameAssets::CreatePlayer(ecs, modelImporter, Vector3::Up * 0.2f))
{ }

void EndlessRunner::ProcessPlayerInput()
{
}

std::vector<Vector3> EndlessRunner::GenerateBoxPoses()
{
	return std::vector<Vector3>();
}

std::vector<Vector3> EndlessRunner::GenerateCoinPoses()
{
	return std::vector<Vector3>();
}

void EndlessRunner::SetAssetsPoses(bool init)
{
}
