// Mate.Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Engine.h"

int main()
{
    Engine* engine = new Engine();
    engine->Initialize();

    while (engine->IsRunning())
    {

    }
}
