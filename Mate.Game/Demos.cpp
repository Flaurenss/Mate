#include "Demos.h"
#include "EngineDemo.h"
#include "game/GameAssets.h"

Demos::Demos(MateEngine& engine) : 
    engine(engine),
    cameraUtils(CameraUtils(CreateCamera())),
    animator(GetPlayerAnimator())
{ }

void Demos::Start()
{
    engine.SetSkybox("skybox", {
        "./Assets/Skybox/right.jpg",
        "./Assets/Skybox/left.jpg",
        "./Assets/Skybox/top.jpg",
        "./Assets/Skybox/bottom.jpg",
        "./Assets/Skybox/front.jpg",
        "./Assets/Skybox/back.jpg" });

    EngineDemo::CreateBaseFloor(engine);
}

void Demos::Update(float deltaTime)
{
    //cameraUtils.ManageFreeCamera(deltaTime);

    if (Input::GetKeyDown(KeyCode::Escape))
    {
        isGameRunning = false;
    }

    if (Input::GetKeyDown(KeyCode::P))
    {
        EngineDemo::PhysicsCubeDemo(engine, 5);
    }

    //DemoAnimations();
    DemoAssets();
}

void Demos::FixedUpdate(float fixedDeltaTime)
{
}

bool Demos::IsGameRunning() const
{
	return isGameRunning;
}

void Demos::DemoAssets()
{
    //Player
    if (Input::GetKeyDown(KeyCode::Z))
    {
        GameAssets::CreateObstacle(engine, Vector3(0, 0.5f, -1), Vector3(), Vector3(2));
    }
    // Box
    if (Input::GetKeyDown(KeyCode::X))
    {
        GameAssets::CreateReward(engine, Vector3(-0.5, 0.5f, 0.5f), Vector3());
    }
    // Coin
    if (Input::GetKeyDown(KeyCode::C))
    {
        GameAssets::CreateReward(engine, Vector3(0.5, 0.5f, 0.5f), Vector3());
    }
    //Coin
    if (Input::GetKeyDown(KeyCode::V))
    {
        GameAssets::CreatePlayer(engine, Vector3::Up * 0.3f, Vector3(0, 0, 0), Vector3(0.5f));
    }
}

void Demos::DemoAnimations()
{
    if (Input::GetKeyDown(KeyCode::Z))
    {
        animator.CurrentAnimationIndex = 1;
    }
    if (Input::GetKeyDown(KeyCode::X))
    {
        animator.CurrentAnimationIndex = 2;
    }
    if (Input::GetKeyDown(KeyCode::C))
    {
        animator.CurrentAnimationIndex = 3;
    }
}

Entity Demos::CreateCamera()
{
    auto camera = engine.CreateEntity();
    camera.AddComponent<TransformComponent>(Vector3(0, 1.5f, 2.5f), Vector3(), Vector3(1));
    camera.AddComponent<CameraComponent>();
    TransformComponent& trans = camera.GetComponent<TransformComponent>();
    trans.LookAt(Vector3(0, 0.65f, 0), trans.GetUp());
    return camera;
}

AnimationComponent& Demos::GetPlayerAnimator()
{
    auto player = GameAssets::CreatePlayer(engine, Vector3::Up * 0.3f, Vector3(0, 0, 0), Vector3(0.5f));
    player.AddComponent<EnableComponent>().Enabled = false;
    return player.GetComponent<AnimationComponent>();
}
