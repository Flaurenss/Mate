#include "GameAssets.h"
#include <AnimationComponent.h>

Entity GameAssets::CreatePlayer(Engine* engine, Vector3 basePos, Vector3 baseRot, Vector3 baseScale)
{
    auto playerModel = "./Assets/Player/character.glb";
    AssetManager::GetInstance().LoadModel("player", playerModel);
    auto player = engine->CreateEntity();
    player.AddComponent<TransformComponent>(basePos, baseRot, baseScale);
    player.AddComponent<MeshComponent>("player");
    player.AddComponent<PhysicsComponent>(MotionType::KINEMATIC);
    player.AddComponent<AnimationComponent>();
    return player;
}

Entity GameAssets::CreateReward(Engine* engine, Vector3 basePos, Vector3 baseRot, Vector3 baseScale)
{
    auto coinModelPath = "./Assets/Environment/Misc/coin.glb";
    AssetManager::GetInstance().LoadModel("coin", coinModelPath);
    Entity coin = engine->CreateEntity();
    auto& coinTrans = coin.AddComponent<TransformComponent>(basePos, baseRot, baseScale);
    coin.AddComponent<MeshComponent>("coin");
    return coin;
}

Entity GameAssets::CreateObstacle(Engine* engine, Vector3 basePos, Vector3 baseRot, Vector3 baseScale)
{
    auto boxModelPath = "./Assets/Environment/Misc/crate-color.glb";
    AssetManager::GetInstance().LoadModel("box", boxModelPath);
    Entity box = engine->CreateEntity();
    auto& boxTrans = box.AddComponent<TransformComponent>(basePos, baseRot, baseScale);
    box.AddComponent<MeshComponent>("box");
    return box;
}
