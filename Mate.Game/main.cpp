// Mate.Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Engine.h"
#include <MeshComponent.h>
#include <TransformComponent.h>
#include <CameraComponent.h>

int main()
{
    //Engine::Test();
    //
    auto modelPath = "C:/Users/loren/Downloads/glTF-Sample-Models-main/glTF-Sample-Models-main/2.0/Avocado/glTF/Avocado.gltf";
    Engine* engine = new Engine();
    ECS& registry = engine->GetRegistry();
    
    auto box = registry.CreateEntity();
    box.AddComponent<TransformComponent>();
    box.AddComponent<MeshComponent>(modelPath);
    TransformComponent& boxTransform = box.GetComponent<TransformComponent>();
    
    auto duck = registry.CreateEntity();
    duck.AddComponent<TransformComponent>();
    duck.AddComponent<MeshComponent>(modelPath);
    TransformComponent& transform = duck.GetComponent<TransformComponent>();
    //transform.Scale(Vector3(5, 5, 5));

    while (engine->IsRunning())
    {
        boxTransform.Scale(Vector3(5, 5, 5));
        boxTransform.Rotate(engine->DeltaTime * 50, Vector3(0, 1, 0));
        boxTransform.Translate(Vector3(0, 0, 0));
        engine->Update();
        engine->Render();
    }
}
