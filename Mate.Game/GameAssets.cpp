#include "GameAssets.h"
#include <AnimationComponent.h>

Entity GameAssets::CreatePlayer(ECS& ecs, Vector3 basePos, Vector3 baseRot, Vector3 baseScale)
{
    auto playerModel = "./Assets/Player/character.glb";
    AssetManager::GetInstance().LoadModel("player", playerModel);
    auto player = ecs.CreateEntity();
    player.AddComponent<TransformComponent>(basePos, baseRot, baseScale);
    player.AddComponent<MeshComponent>("player");
    player.AddComponent<PhysicsComponent>(MotionType::KINEMATIC);
    player.AddComponent<AnimationComponent>();
    return player;
}

Entity GameAssets::CreateReward(ECS& ecs, Vector3 basePos, Vector3 baseRot, Vector3 baseScale)
{
    auto coinModelPath = "./Assets/Environment/Misc/coin.glb";
    AssetManager::GetInstance().LoadModel("coin", coinModelPath);
    Entity coin = ecs.CreateEntity();
    auto& coinTrans = coin.AddComponent<TransformComponent>(basePos, baseRot, baseScale);
    coin.AddComponent<MeshComponent>("coin");
    return coin;
}

Entity GameAssets::CreateObstacle(ECS& ecs, Vector3 basePos, Vector3 baseRot, Vector3 baseScale)
{
    auto boxModelPath = "./Assets/Environment/Misc/crate-color.glb";
    AssetManager::GetInstance().LoadModel("box", boxModelPath);
    Entity box = ecs.CreateEntity();
    auto& boxTrans = box.AddComponent<TransformComponent>(basePos, baseRot, baseScale);
    box.AddComponent<MeshComponent>("box");
    return box;
}
