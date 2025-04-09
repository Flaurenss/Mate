// Mate.Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Mate.h>

int main()
{
    auto avocadoModel = "./Assets/Avocado/Avocado.gltf";
    Engine* engine = new Engine();
    ECS& registry = engine->GetRegistry();
    
    auto floorMesh = PrimitivesHelper::CreatePlane();
    auto floor = registry.CreateEntity();
    floor.AddComponent<TransformComponent>();
    floor.AddComponent<MeshComponent>(floorMesh);
    TransformComponent& floorTrans = floor.GetComponent<TransformComponent>();
    floorTrans.Scale(Vector3(10, 1, 10));

    auto avocado = registry.CreateEntity();
    avocado.AddComponent<TransformComponent>();
    avocado.AddComponent<MeshComponent>(avocadoModel);
    TransformComponent& boxTransform = avocado.GetComponent<TransformComponent>();
    boxTransform.SetPosition(Vector3(0, 0.01f, 0));
    boxTransform.Scale(Vector3(5, 5, 5));
    
    float rotationSpeedDegrees = 90.0f;
    float movementSpeedUnits = 0.1f;
    while (engine->IsRunning())
    {
        float deltaTime = engine->DeltaTime;
        
        float deltaRotationY = rotationSpeedDegrees * deltaTime;
        boxTransform.Rotate(Vector3(0, deltaRotationY, 0));
        
        Vector3 currentPosition = boxTransform.position;
        float deltaMovementX = movementSpeedUnits * deltaTime;
        boxTransform.Translate(Vector3(deltaMovementX, 0.0f, 0.0f));

        engine->Update();
        engine->Render();
    }
}
