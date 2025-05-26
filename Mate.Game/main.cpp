#include "Mate.h"
#include "Game/EndlessRunner.h"

int main()
{
    auto engine = std::make_unique<MateEngine>();
    //engine->SetRenderDebugMode();
    //engine->ShowFrames();
    auto game = EndlessRunner(engine.get());
    engine->Run(game);
 }