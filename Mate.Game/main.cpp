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
    auto camera = registry.CreateEntity();
    camera.AddComponent<TransformComponent>();
    camera.AddComponent<CameraComponent>();
    
    auto box = registry.CreateEntity();
    box.AddComponent<TransformComponent>();
    box.AddComponent<MeshComponent>(modelPath);
    
    auto duck = registry.CreateEntity();
    duck.AddComponent<TransformComponent>();
    duck.AddComponent<MeshComponent>(modelPath);
    TransformComponent& transform = duck.GetComponent<TransformComponent>();
    while (engine->IsRunning())
    {
        transform.Rotate(-180, Vector3(0, 1, 0));
        transform.Translate(Vector3(1.5f, 0, 0));
        engine->Update();
        engine->Render();
    }
}
