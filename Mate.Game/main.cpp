// Mate.Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Mate.h"
#include "Game/EndlessRunner.h"

int main()
{
    auto engine = std::make_unique<Engine>();
    engine->SetRenderDebugMode(true);
    auto game = EndlessRunner(engine.get());
    engine->Run(game);
 }