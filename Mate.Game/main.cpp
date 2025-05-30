#include "Mate.h"
#include "Game/EndlessRunner.h"
#include "Demos.h"

int main()
{
    MateEngine engine;
    //engine.SetRenderDebugMode();
    engine.ShowFrames();
    //auto game = EndlessRunner(engine);
    auto demos = Demos(engine);
    engine.Run(demos);
 }