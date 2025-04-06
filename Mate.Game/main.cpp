// Mate.Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Engine.h"
#include <MeshComponent.h>
#include <TransformComponent.h>
#include <CameraComponent.h>

int main()
{
    Engine* engine = new Engine();
    //engine->Test();
    //
    ECS& registry = engine->GetRegistry();
    auto camera = registry.CreateEntity();
    camera.AddComponent<TransformComponent>();
    camera.AddComponent<CameraComponent>();
    
    auto box = registry.CreateEntity();
    box.AddComponent<TransformComponent>();
    box.AddComponent<MeshComponent>("E:/TFG/Models/Kenney/Models/GLB format/character-female-d.glb");
    
    auto duck = registry.CreateEntity();
    duck.AddComponent<TransformComponent>();
    duck.AddComponent<MeshComponent>("E:/TFG/Models/glTF-Sample-Assets-main/Models/Duck/glTF/Duck.gltf");
    TransformComponent& transform = duck.GetComponent<TransformComponent>();
    transform.Rotate(-180, Vector3(0, 1, 0));
    transform.Translate(Vector3(1.5f, 0, 0));
    while (engine->IsRunning())
    {
        engine->Update();
        engine->Render();
    }
}
