#include "EngineDemo.h"
#include <Mate.h>
#include "Game/GameAssets.h"

void EngineDemo::CreateBaseFloor(MateEngine* engine)
{
    auto floorModel = PrimitivesHelper::CreatePlane();
    AssetManager::GetInstance().RegisterModel("primitive::floor", floorModel);
    auto floor = engine->CreateEntity();
    floor.AddComponent<TransformComponent>();
    floor.AddComponent<PhysicsComponent>(MotionType::KINEMATIC);
    floor.AddComponent<MeshComponent>("primitive::floor");
    TransformComponent& floorTrans = floor.GetComponent<TransformComponent>();
    floorTrans.Scale = Vector3(20, 1, 20);
}

void EngineDemo::PhysicsCubeDemo(MateEngine* engine)
{
    CreateBaseFloor(engine);

    const float spacing = 1;
    const int cubeNumber = 2;
    for (int x = 0; x < cubeNumber; x++)
    {
        for (int y = 0; y < cubeNumber; y++)
        {
            for (int z = 0; z < cubeNumber; z++)
            {
                Vector3 pos(
                    (x - 1) * spacing,
                    y * spacing + 1.0f,
                    (z - 1) * spacing 
                );

                auto box = GameAssets::CreateObstacle(engine, pos);
                box.AddComponent<PhysicsComponent>();
            }
        }
    }
}
