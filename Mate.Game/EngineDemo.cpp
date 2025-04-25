#include "EngineDemo.h"
#include <Mate.h>
#include "GameAssets.h"

void EngineDemo::CreateBaseFloor(ECS& ecs)
{
    auto floorMesh = PrimitivesHelper::CreatePlane();
    auto floor = ecs.CreateEntity();
    floor.AddComponent<TransformComponent>();
    floor.AddComponent<PhysicsComponent>();
    floor.AddComponent<MeshComponent>(floorMesh);
    TransformComponent& floorTrans = floor.GetComponent<TransformComponent>();
    floorTrans.Scale = Vector3(20, 1, 20);
}

void EngineDemo::PhysicsCubeDemo(ECS& ecs, ModelImporter& modelImporter)
{
    CreateBaseFloor(ecs);

    const float spacing = 1;
    const int cubeNumber = 9;
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

                GameAssets::CreateObstacle(ecs, modelImporter, pos);
            }
        }
    }
}
