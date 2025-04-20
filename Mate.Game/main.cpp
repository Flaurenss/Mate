// Mate.Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
// TODO: add pch with Mate.h
#include <Mate.h>
#include "EnvironmentAssets.h"

float yaw = -90.0f;
float pitch = -20.0f;
float lastX = 0;
float lastY = 0;
bool firstMouse = true;

void CreateFloor(ECS& ecs);
Entity CreateCamera(ECS& ecs);
TransformComponent& CreatePlayer(ECS& ecs);
TransformComponent& CreateMisc(ECS& ecs);

EnvironmentAsset CreateMovableMisc(ECS& registry, int i);
void ManageMovableMisc(std::vector<EnvironmentAsset>& assets, float deltaTime);

void ManagePlayerInputRails(TransformComponent& transform, float deltaTime, Vector3 originalPos);
void ManageFreeCamera(CameraComponent& cameraComponent, TransformComponent& transformCamera, float deltaTime);
void ManagePlayerInput(TransformComponent& transform, float deltaTime);

int main()
{
    Engine* engine = new Engine();
    ECS& ecs = engine->GetRegistry();
    
    auto camera = CreateCamera(ecs);
    TransformComponent& cameraTransform = camera.GetComponent<TransformComponent>();
    CameraComponent& cameraComponent = camera.GetComponent<CameraComponent>();
    
    TransformComponent& playerTransform = CreatePlayer(ecs);
    std::vector<EnvironmentAsset> environmentAssets;
    for (auto i = 0; i < 1; i++)
    {
        environmentAssets.push_back(CreateMovableMisc(ecs, i));
    }

    
    float rotationSpeedDegrees = 90.0f;
    float movementSpeedUnits = 0.1f;
    Vector3 originalPos = playerTransform.Position;
    while (engine->IsRunning())
    {
        float deltaTime = engine->DeltaTime;
        //ManagePlayerInputRails(playerTransform, deltaTime, originalPos);
        ManageFreeCamera(cameraComponent, cameraTransform, deltaTime);
        ManageMovableMisc(environmentAssets, deltaTime);

        engine->Update();
        engine->Render();
    }
}

void CreateFloor(ECS& ecs)
{
    auto floorMesh = PrimitivesHelper::CreatePlane();
    auto floor = ecs.CreateEntity();
    floor.AddComponent<TransformComponent>();
    floor.AddComponent<MeshComponent>(floorMesh);
    TransformComponent& floorTrans = floor.GetComponent<TransformComponent>();
    floorTrans.Scale = Vector3(5, 1, 5);
}

Entity CreateCamera(ECS& ecs)
{
    auto camera = ecs.CreateEntity();
    camera.AddComponent<TransformComponent>(Vector3(0, 1.5f, 2.5f), Vector3(), Vector3(1));
    camera.AddComponent<CameraComponent>();
    CameraComponent& cameraComponent = camera.GetComponent<CameraComponent>();
    TransformComponent& trans = camera.GetComponent<TransformComponent>();
    trans.LookAt(Vector3(), trans.GetUp());
    return camera;
}

TransformComponent& CreatePlayer(ECS& ecs)
{
    auto playerModel = "./Assets/Player/character.glb";
    auto player = ecs.CreateEntity();
    player.AddComponent<TransformComponent>(Vector3(0, 0.01f, 0), Vector3(0, -180, 0), Vector3(0.5f));
    player.AddComponent<MeshComponent>(playerModel);
    TransformComponent& playerTransform = player.GetComponent<TransformComponent>();
    playerTransform.SetPosition(Vector3(0, 0.01f, 0));
    //player.Destroy();
    return playerTransform;
}

TransformComponent& CreateMisc(ECS& ecs)
{
    auto avocadoModel = "./Assets/Avocado/Avocado.gltf";
    auto avocado = ecs.CreateEntity();
    avocado.AddComponent<TransformComponent>();
    avocado.AddComponent<MeshComponent>(avocadoModel);
    TransformComponent& avcTransform = avocado.GetComponent<TransformComponent>();
    avcTransform.SetPosition(Vector3(0, 0.01f, 0));
    avcTransform.DoScale(2);
    return avcTransform;
}

EnvironmentAsset CreateMovableMisc(ECS& ecs, int i)
{
    auto coinModel = "./Assets/Environment/Misc/coin.glb";
    auto boxModel = "./Assets/Environment/Misc/crate-color.glb";
    auto roadModel = "./Assets/Environment/Road/road-straight.glb";
    int separation = i;


    auto coin = ecs.CreateEntity();
    auto coinPos = Vector3(-1.5, 0.2f, -1);
    coin.AddComponent<TransformComponent>(coinPos, Vector3(0, -90, 0), Vector3::One);
    coin.AddComponent<MeshComponent>(coinModel);
    Part coinPart{ coin, coinPos, EnvironmentType::Reward };

    auto box = ecs.CreateEntity();
    auto boxPos = Vector3(0.2f, 0, 1);
    box.AddComponent<TransformComponent>(boxPos);
    box.AddComponent<MeshComponent>(boxModel);
    Part boxPart{ box, boxPos, EnvironmentType::Obstacle };

    auto road = ecs.CreateEntity();
    auto roadPos = Vector3(0, 0, 0 + (-i * 6));
    road.AddComponent<TransformComponent>(roadPos, Vector3::Zero, Vector3(5, 1, 10));
    road.AddComponent<MeshComponent>(roadModel);
    Part floorPart{ road, roadPos, EnvironmentType::Floor };

    // Agrupar en EnvironmentPart
    EnvironmentPart envPart{ floorPart, {boxPart}, {coinPart} };
    EnvironmentAsset asset {envPart};
    return asset;
}

void ManageMovableMisc(std::vector<EnvironmentAsset>& assets, float deltaTime)
{
    float speed = deltaTime * 1.5f;
    for (auto asset : assets)
    {
        TransformComponent& floorTrans = asset.floor.floorPart.entity.GetComponent<TransformComponent>();
        if (floorTrans.Position.z > 5)
        {
            floorTrans.SetPosition(asset.floor.floorPart.originalPos);
            for (auto coll : asset.floor.collisions)
            {
                TransformComponent& collTrans = coll.entity.GetComponent<TransformComponent>();
                collTrans.SetPosition(coll.originalPos);
            }


            for (auto reward : asset.floor.rewards)
            {
                TransformComponent& rewardTrans = reward.entity.GetComponent<TransformComponent>();
                rewardTrans.SetPosition(reward.originalPos);
            }
        }
        else
        {
            auto movement = -Vector3::Forward * speed;
            floorTrans.Translate(movement);
            for (auto coll : asset.floor.collisions)
            {
                TransformComponent& collTrans = coll.entity.GetComponent<TransformComponent>();
                collTrans.Translate(movement);
            }

            for (auto reward : asset.floor.rewards)
            {
                TransformComponent& rewardTrans = reward.entity.GetComponent<TransformComponent>();
                rewardTrans.Translate(movement);
            }
        }
    }
}

void ManagePlayerInputRails(TransformComponent& transform, float deltaTime, Vector3 originalPos)
{
    float space = 1.5f;
    float maxRight = originalPos.x + space;
    float maxLeft = originalPos.x - space;

    if (Input::GetKeyDown(KeyCode::D))
    {
        float newX = transform.Position.x + space;
        if (newX <= maxRight)
        {
            transform.Position.x = newX;
        }
    }

    if (Input::GetKeyDown(KeyCode::A))
    {
        float newX = transform.Position.x - space;
        if (newX >= maxLeft)
        {
            transform.Position.x = newX;
        }
    }
}

void ManageFreeCamera(CameraComponent& cameraComponent, TransformComponent& cameraTransform, float deltaTime)
{
    float xPos = Input::MousePosition.x;
    float yPos = Input::MousePosition.y;
    float sensitivity = 0.1f;

    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xoffset = lastX - xPos;
    float yoffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    Vector3 currentEuler = cameraTransform.EulerAngles;
    currentEuler.y = yaw;
    currentEuler.x = pitch;
    currentEuler.z = 0;
    cameraTransform.EulerAngles = currentEuler;

    float speed = deltaTime * 2.5f;
    Vector3 direction = Vector3::Zero;
    Vector3 transformForward = cameraTransform.GetForward();
    Vector3 transformRight = cameraTransform.GetRight();

    if (Input::GetKey(KeyCode::W)) direction += transformForward;
    if (Input::GetKey(KeyCode::S)) direction -= transformForward;
    if (Input::GetKey(KeyCode::A)) direction -= transformRight;
    if (Input::GetKey(KeyCode::D)) direction += transformRight;

    cameraTransform.Translate(direction.normalize() * speed);
}

void ManagePlayerInput(TransformComponent& transform, float deltaTime)
{
    auto velocity = deltaTime * 1.1f;
    Vector3 direction = Vector3::Zero;
    if (Input::GetKey(KeyCode::W))
    {
        direction += Vector3::Forward;
    }

    if (Input::GetKey(KeyCode::S))
    {
        direction += Vector3::Back;
    }

    if (Input::GetKey(KeyCode::D))
    {
        direction += Vector3::Right;
    }

    if (Input::GetKey(KeyCode::A))
    {
        direction += Vector3::Left;
    }

    transform.Translate(direction.normalize() * velocity);
}