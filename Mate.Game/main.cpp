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
    boxTransform.Translate(Vector3(-1, 0, 0));
    
    //auto duck = registry.CreateEntity();
    //duck.AddComponent<TransformComponent>();
    //duck.AddComponent<MeshComponent>(modelPath);
    //TransformComponent& transform = duck.GetComponent<TransformComponent>();
    //transform.Scale(Vector3(5, 5, 5));

    while (engine->IsRunning())
    {
        //transform.Rotate(engine->DeltaTime * 50, Vector3(0, 1, 0));
        //transform.Translate(Vector3(engine->DeltaTime, 0, 0));
        //boxTransform.Translate(Vector3(0, 0, 0));
        engine->Update();
        engine->Render();
    }
}
