# How to use Mate Engine

1. Build this project and link `Mate.lib` to your project (you can check `Mate.Game` how it is done).
2. Include [`Mate.h`](./Mate.h) to your project.
   > This is your entry point to the engine where all the basic `systems` and `components` can be accesed.
3. Create a new class that inherits from [`IGame.h`](./IGame.h)
   > Add some logic to it. The interface follows a similar pattern than other game engines. e.g. `Monobehavior from Unity.`
4. Send it to [`MateEngine`](./MateEngine.h):
   ```
    auto engine = new MateEngine();
    auto game = IGameImplementation(engine);
    engine->Run(game);
   ```
4. `Enjoy!`