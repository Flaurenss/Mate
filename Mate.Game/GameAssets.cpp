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

Entity GameAssets::CreateReward(ECS& ecs, ModelImporter& modelImporter, Vector3 basePos, Vector3 baseRot, Vector3 baseScale)
{
    auto coinModelPath = "./Assets/Environment/Misc/coin.glb";
    auto coinMeshes = modelImporter.Load(coinModelPath);
    Entity coin = ecs.CreateEntity();
    auto& coinTrans = coin.AddComponent<TransformComponent>(basePos, baseRot, baseScale);
    coin.AddComponent<MeshComponent>(coinMeshes);
    return coin;
}

Entity GameAssets::CreateObstacle(ECS& ecs, ModelImporter& modelImporter, Vector3 basePos, Vector3 baseRot, Vector3 baseScale)
{
    auto boxModelPath = "./Assets/Environment/Misc/crate-color.glb";
    auto boxMeshes = modelImporter.Load(boxModelPath);
    Entity box = ecs.CreateEntity();
    auto& boxTrans = box.AddComponent<TransformComponent>(basePos, baseRot, baseScale);
    box.AddComponent<MeshComponent>(boxMeshes);
    return box;
}
