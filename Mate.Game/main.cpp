// Mate.Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Engine.h"
#include <MeshComponent.h>
#include <TransformComponent.h>

int main()
{
    Engine* engine = new Engine();

    auto registry = engine->GetECS();
    auto box = registry.CreateEntity();
    box.AddComponent<TransformComponent>();
    box.AddComponent<MeshComponent>();

    while (engine->IsRunning())
    {
        engine->ProcessInput();
        engine->Update();
        engine->Render();
    }
}
