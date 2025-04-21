// Mate.Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
// TODO: add pch with Mate.h
#include <Mate.h>
#include "EnvironmentAssets.h"
#include <deque>

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
void ManageMovableMisc(std::deque<EnvironmentAsset>& assets, float deltaTime);
void ResetEnvironmentPart(EnvironmentPart& part, const Vector3& newFloorPos);

void ManagePlayerInputRails(TransformComponent& transform, PlayerRailState& state, float playerOriginalX, float deltaTime);
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
    std::deque<EnvironmentAsset> environmentAssets;
    for (auto i = 0; i < 20; i++)
    {
        environmentAssets.push_back(CreateMovableMisc(ecs, i));
    }

    
    float rotationSpeedDegrees = 90.0f;
    float movementSpeedUnits = 0.1f;
    Vector3 originalPos = playerTransform.Position;
    PlayerRailState railState;
    railState.targetX = playerTransform.Position.x;
    railState.currentRail = 0;
    while (engine->IsRunning())
    {
        float deltaTime = engine->DeltaTime;
        ManagePlayerInputRails(playerTransform, railState, originalPos.x, deltaTime);
        //ManageFreeCamera(cameraComponent, cameraTransform, deltaTime);
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
    trans.LookAt(Vector3(0, 0.65f, 0), trans.GetUp());
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

    auto road = ecs.CreateEntity();
    auto roadPos = Vector3(0, 0, 0 + (-i * 10));
    road.AddComponent<TransformComponent>(roadPos, Vector3::Zero, Vector3(5, 1, 10));
    road.AddComponent<MeshComponent>(roadModel);
    Part floorPart{ road, roadPos, EnvironmentType::Floor };

    Vector3 coinOffset(-1.5f, 0.2f, -1.0f);
    Vector3 coinPos = roadPos + coinOffset;
    Entity coin = ecs.CreateEntity();
    coin.AddComponent<TransformComponent>(coinPos, Vector3(0, -90, 0), Vector3::One);
    coin.AddComponent<MeshComponent>(coinModel);
    Part coinPart{ coin, coinPos, EnvironmentType::Reward };

    Vector3 boxOffset(0, 0, 1.0f);
    Vector3 boxPos = roadPos + boxOffset;
    Entity box = ecs.CreateEntity();
    box.AddComponent<TransformComponent>(boxPos);
    box.AddComponent<MeshComponent>(boxModel);
    Part boxPart{ box, boxPos, EnvironmentType::Obstacle };

    EnvironmentPart envPart{ floorPart, {boxPart}, {coinPart} };
    EnvironmentAsset asset {envPart};
    return asset;
}

void ManageMovableMisc(std::deque<EnvironmentAsset>& assets, float deltaTime)
{
    const float speed = deltaTime * 7.5f;
    const float blockLength = 10.0f;

    // Check if recent block is out of bounds
    auto& firstBlock = assets.front();
    TransformComponent& firstFloorTrans = firstBlock.floor.floorPart.entity.GetComponent<TransformComponent>();

    if (firstFloorTrans.Position.z > 10.0f)
    {
        // Get last floor pos
        auto& lastBlock = assets.back();
        Vector3 lastFloorPos = lastBlock.floor.floorPart.entity.GetComponent<TransformComponent>().Position;

        // Calculate new pos and reset children
        Vector3 newBasePos = lastFloorPos - Vector3(0, 0, blockLength);
        ResetEnvironmentPart(firstBlock.floor, newBasePos);

        // Push to end queue
        assets.push_back(firstBlock);
        assets.pop_front();
    }

    // Move all blocks
    Vector3 movement = -Vector3::Forward * speed;
    for (auto& asset : assets)
    {
        asset.floor.floorPart.entity.GetComponent<TransformComponent>().Translate(movement);

        for (auto& coll : asset.floor.collisions)
        {
            coll.entity.GetComponent<TransformComponent>().Translate(movement);
        }

        for (auto& reward : asset.floor.rewards)
        {
            reward.entity.GetComponent<TransformComponent>().Translate(movement);
        }
    }
}

void ResetEnvironmentPart(EnvironmentPart& part, const Vector3& newFloorPos)
{
    Vector3 oldFloorPos = part.floorPart.originalPos;

    part.floorPart.entity.GetComponent<TransformComponent>().SetPosition(newFloorPos);
    part.floorPart.originalPos = newFloorPos;

    for (auto& coll : part.collisions)
    {
        Vector3 offset = coll.originalPos - oldFloorPos;
        Vector3 newPos = newFloorPos + offset;
        coll.entity.GetComponent<TransformComponent>().SetPosition(newPos);
        coll.originalPos = newPos;
    }

    for (auto& reward : part.rewards)
    {
        Vector3 offset = reward.originalPos - oldFloorPos;
        Vector3 newPos = newFloorPos + offset;
        reward.entity.GetComponent<TransformComponent>().SetPosition(newPos);
        reward.originalPos = newPos;
    }
}


void ManagePlayerInputRails(TransformComponent& transform, PlayerRailState& state, float playerOriginalX, float deltaTime)
{
    const float space = 1.5f;
    const float speed = 5.0f;

    if (Input::GetKeyDown(KeyCode::D) && state.currentRail < 1)
    {
        state.currentRail++;
        state.targetX = state.currentRail * space;
    }

    if (Input::GetKeyDown(KeyCode::A) && state.currentRail > -1)
    {
        state.currentRail--;
        state.targetX = state.currentRail * space;
    }

    float currentX = transform.Position.x;
    float deltaX = state.targetX - currentX;

    if (std::abs(deltaX) > 0.01f)
    {
        float direction = (deltaX > 0) ? 1.0f : -1.0f;
        float move = speed * deltaTime;

        if (std::abs(move) >= std::abs(deltaX))
        {
            transform.SetPosition(Vector3(state.targetX, transform.Position.y, transform.Position.z));
        }
        else
        {
            transform.Translate(Vector3::Right * move * direction);
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

    float speed = deltaTime * 5.0f;
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