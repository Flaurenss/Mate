#include "GameAssets.h"

Entity GameAssets::CreatePlayer(ECS& ecs, ModelImporter& modelImporter, Vector3 basePos)
{
    auto playerModel = "./Assets/Player/character.glb";
    auto modelMeshes = modelImporter.Load(playerModel);
    auto player = ecs.CreateEntity();
    player.AddComponent<TransformComponent>(basePos, Vector3(0, -180, 0), Vector3(0.5f));
    player.AddComponent<MeshComponent>(modelMeshes);
    player.AddComponent<PhysicsComponent>(MotionType::KINEMATIC);
    return player;
}

void GameAssets::CreateReward(ECS& ecs, ModelImporter& modelImporter, Vector3 basePos)
{
    auto coinModelPath = "./Assets/Environment/Misc/coin.glb";
    auto coinMeshes = modelImporter.Load(coinModelPath);
    Entity coin = ecs.CreateEntity();
    coin.AddComponent<EnableComponent>().Enabled = false;
    auto& coinTrans = coin.AddComponent<TransformComponent>(basePos);
    coin.AddComponent<MeshComponent>(coinMeshes);
}

void GameAssets::CreateObstacle(ECS& ecs, ModelImporter& modelImporter, Vector3 basePos)
{
    auto boxModelPath = "./Assets/Environment/Misc/crate-color.glb";
    auto boxMeshes = modelImporter.Load(boxModelPath);
    Entity box = ecs.CreateEntity();
    auto& boxTrans = box.AddComponent<TransformComponent>(basePos, Vector3(0), Vector3(1));
    box.AddComponent<MeshComponent>(boxMeshes);
    auto& phy = box.AddComponent<PhysicsComponent>(MotionType::STATIC);
    phy.SetIsSensor(true);
}
