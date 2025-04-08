// Mate.Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Engine.h"
#include <MeshComponent.h>
#include <TransformComponent.h>
#include <CameraComponent.h>
#include "PrimitivesHelper.h"

int main()
{
    //Engine::Test();
    //
    auto modelPath = "C:/Users/loren/Downloads/glTF-Sample-Models-main/glTF-Sample-Models-main/2.0/Avocado/glTF/Avocado.gltf";
    Engine* engine = new Engine();
    ECS& registry = engine->GetRegistry();
    
    auto floorMesh = PrimitivesHelper::CreatePlane();
    auto floor = registry.CreateEntity();
    floor.AddComponent<TransformComponent>();
    floor.AddComponent<MeshComponent>(floorMesh);
    TransformComponent& floorTrans = floor.GetComponent<TransformComponent>();
    floorTrans.Scale(Vector3(10, 1, 10));

    auto box = registry.CreateEntity();
    box.AddComponent<TransformComponent>();
    box.AddComponent<MeshComponent>(modelPath);
    TransformComponent& boxTransform = box.GetComponent<TransformComponent>();
    boxTransform.Scale(Vector3(5, 5, 5));
    
    //auto duck = registry.CreateEntity();
    //duck.AddComponent<TransformComponent>();
    //duck.AddComponent<MeshComponent>(modelPath);
    //TransformComponent& transform = duck.GetComponent<TransformComponent>();
    //transform.Scale(Vector3(5, 5, 5));
    float rotationSpeedDegrees = 90.0f;
    float movementSpeedUnits = 0.1f;
    while (engine->IsRunning())
    {
        float deltaTime = engine->DeltaTime;
        
        float deltaRotationY = rotationSpeedDegrees * deltaTime;
        boxTransform.Rotate(Vector3(0, deltaRotationY, 0));
        
        Matrix4 currentTransform = boxTransform.GetTransform();
        Vector3 currentPosition(currentTransform[12], currentTransform[13], currentTransform[14]);

        float deltaMovementX = movementSpeedUnits * deltaTime;
        //Vector3 newPosition = currentPosition + Vector3(deltaMovementX, 0.0f, 0.0f);
        //boxTransform.SetPosition(newPosition);

        boxTransform.Translate(Vector3(deltaMovementX, 0.0f, 0.0f));

        engine->Update();
        engine->Render();
    }
}
