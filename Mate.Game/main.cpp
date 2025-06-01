#include "Mate.h"
#include "Game/EndlessRunner.h"

int main()
{
    MateEngine engine;
    //engine.SetRenderDebugMode();
    //engine.ShowFrames();
    auto game = EndlessRunner(engine);
    engine.Run(game);
 }