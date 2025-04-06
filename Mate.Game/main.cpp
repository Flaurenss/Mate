// Mate.Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Engine.h"
#include <MeshComponent.h>
#include <TransformComponent.h>

int main()
{
    Engine* engine = new Engine();

    ECS& registry = engine->GetRegistry();
    auto box = registry.CreateEntity();
    box.AddComponent<TransformComponent>();
    box.AddComponent<MeshComponent>("E:/TFG/Models/Kenney/Models/GLB format/character-female-d.glb");

    while (engine->IsRunning())
    {
        engine->ProcessInput();
        engine->Update();
        engine->Render();
    }
}
