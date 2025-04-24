// Mate.Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
// TODO: add pch with Mate.h
#include <Mate.h>
#include "EnvironmentAssets.h"
#include <deque>
#include <random>

float yaw = 45.0f;
float pitch = 0;
float lastX = 0;
float lastY = 0;
bool firstMouse = true;
const int ROAD_LENGHT = 10;
const int START_COINS = 10;
const int START_OBSTACLES = 3;

void CreateFloor(ECS& ecs);
Entity CreateCamera(ECS& ecs);
TransformComponent& CreatePlayer(ECS& ecs);
TransformComponent& CreateMisc(ECS& ecs);

std::deque<EnvironmentPart> CreateEnvironment(ECS& ecs);
EnvironmentPart CreateMovableMisc(ECS& registry, int i);
Part CreateCoin(Vector3 roadPos, float xOffset, float zOffset, ECS& ecs);
Part CreateBox(Vector3 roadPos, float xOffset, float zOffset, ECS& ecs);

void ManageMovableMisc(std::deque<EnvironmentPart>& assets, float deltaTime);
void ResetEnvironmentPart(EnvironmentPart& part, const Vector3& newFloorPos);
void RedoPart(EnvironmentPart& part);

void ManagePlayerInputRails(TransformComponent& transform, PlayerRailState& state, float playerOriginalX, float deltaTime);
void ManageFreeCamera(CameraComponent& cameraComponent, TransformComponent& transformCamera, float deltaTime);
void ManagePlayerInput(TransformComponent& transform, float deltaTime);

int main()
{
    auto engine = std::make_unique<Engine>();
    ECS& ecs = engine->GetRegistry();
    
    auto camera = CreateCamera(ecs);
    TransformComponent& cameraTransform = camera.GetComponent<TransformComponent>();
    CameraComponent& cameraComponent = camera.GetComponent<CameraComponent>();
    
    TransformComponent& playerTransform = CreatePlayer(ecs);
    std::deque<EnvironmentPart> environmentAssets = CreateEnvironment(ecs);
    
    float rotationSpeedDegrees = 90.0f;
    float movementSpeedUnits = 0.1f;
    Vector3 originalPos = playerTransform.Position;
    PlayerRailState railState;
    railState.targetX = playerTransform.Position.x;
    railState.currentRail = 0;
    while (engine->IsRunning())
    {
        float deltaTime = engine->DeltaTime;
        //ManageFreeCamera(cameraComponent, cameraTransform, deltaTime);
        ManagePlayerInputRails(playerTransform, railState, originalPos.x, deltaTime);
        ManageMovableMisc(environmentAssets, deltaTime);

        //if (Input::GetKeyDown(KeyCode::Space))
        //{
        //    RedoPart(environmentAssets.back());
        //}
        engine->Update();
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

std::deque<EnvironmentPart> CreateEnvironment(ECS& ecs)
{
    std::deque<EnvironmentPart> environmentParts;
    for (auto i = 0; i < ROAD_LENGHT; i++)
    {
        auto envPart = CreateMovableMisc(ecs, i);
        RedoPart(envPart);
        environmentParts.push_back(envPart);
    }

    return environmentParts;
}

EnvironmentPart CreateMovableMisc(ECS& ecs, int i)
{
    auto roadModel = "./Assets/Environment/Road/road-straight.glb";

    auto road = ecs.CreateEntity();
    auto roadPos = Vector3(0, 0, 0 + (-i * 10));
    auto& roadTrans = road.AddComponent<TransformComponent>(roadPos, Vector3::Zero, Vector3(5, 1, 10));
    road.AddComponent<MeshComponent>(roadModel);
    Part floorPart{ road, roadPos, roadTrans, EnvironmentType::Floor };
    
    std::vector<Part> rewards;
    std::vector<Part> obstacles;
    if (i != 0)
    {
        for (auto i = 0; i < START_OBSTACLES; i ++)
        {
            auto boxPart = CreateBox(roadPos, 0, 0, ecs);
            obstacles.push_back(boxPart);
        }

        for (auto i = 0; i < START_COINS; i++)
        {
            auto coinPart = CreateCoin(roadPos, 0, 0, ecs);
            rewards.push_back(coinPart);
        }
    }

    EnvironmentPart envPart{ floorPart, obstacles, rewards};
    return envPart;
}

Part CreateCoin(Vector3 roadPos, float xOffset, float zOffset, ECS& ecs)
{
    auto coinModel = "./Assets/Environment/Misc/coin.glb";
    Vector3 coinOffset(xOffset, 0.2f, zOffset);
    Vector3 coinPos = roadPos + coinOffset;
    Entity coin = ecs.CreateEntity();
    coin.AddComponent<EnableComponent>().Enabled = false;
    auto& coinTrans = coin.AddComponent<TransformComponent>(coinPos, Vector3(0, -90, 0), Vector3::One);
    coin.AddComponent<MeshComponent>(coinModel);
    Part coinPart{ coin, coinPos, coinTrans, EnvironmentType::Reward };
    return coinPart;
}

Part CreateBox(Vector3 roadPos, float xOffset, float zOffset, ECS& ecs)
{
    // Limits for item placement -1.5f ----- -4 ----- 1.5f
    auto boxModel = "./Assets/Environment/Misc/crate-color.glb";
    Vector3 boxOffset(xOffset, 0, zOffset);
    Vector3 boxPos = roadPos + boxOffset;
    Entity box = ecs.CreateEntity();
    box.AddComponent<EnableComponent>().Enabled = false;
    auto& boxTrans = box.AddComponent<TransformComponent>(boxPos);
    box.AddComponent<MeshComponent>(boxModel);
    Part boxPart{ box, boxPos, boxTrans, EnvironmentType::Obstacle };
    return boxPart;
}

void ManageMovableMisc(std::deque<EnvironmentPart>& environmentPartsQueue, float deltaTime)
{
    const float blockLength = 10.0f;

    // Check if recent block is out of bounds
    auto& firstBlock = environmentPartsQueue.front();
    TransformComponent& firstFloorTrans = firstBlock.GetFloorTransform();

    if (firstFloorTrans.Position.z > 10.0f)
    {
        // Get last floor pos
        auto& lastBlock = environmentPartsQueue.back();
        Vector3 lastFloorPos = lastBlock.GetFloorTransform().Position;

        // Calculate new pos and reset children
        Vector3 newBasePos = lastFloorPos - Vector3(0, 0, blockLength);
        ResetEnvironmentPart(firstBlock, newBasePos);

        // Push to end queue
        environmentPartsQueue.push_back(firstBlock);
        environmentPartsQueue.pop_front();
    }

    // Move all blocks
    const float speed = deltaTime * 7.5f;
    const float rotationSpeed = deltaTime * 200;
    Vector3 movement = -Vector3::Forward * speed;
    for (auto& environmentPart : environmentPartsQueue)
    {
        environmentPart.floorPart.transformComponent.Translate(movement);

        for (auto& coll : environmentPart.obstacles)
        {
            coll.transformComponent.Translate(movement);
        }

        for (auto& reward : environmentPart.rewards)
        {
            reward.transformComponent.Rotate(Vector3::Up * rotationSpeed);
            reward.transformComponent.Translate(movement);
        }
    }
}

void ResetEnvironmentPart(EnvironmentPart& part, const Vector3& newFloorPos)
{
    Vector3 oldFloorPos = part.floorPart.originalPos;

    part.floorPart.transformComponent.SetPosition(newFloorPos);
    part.floorPart.originalPos = newFloorPos;

    RedoPart(part);
}

// Used to reposition assets again when floor reset happens
void RedoPart(EnvironmentPart& part)
{
    if (part.obstacles.empty() && part.rewards.empty())
    {
        return;
    }

    static std::vector<float> columns = { -1.5f, 0.0f, 1.5f };
    static std::default_random_engine rng(std::random_device{}());

    // Calculate coins to use
    int maxCoinsAvailable = part.rewards.size();
    int coinsUsed = 0;

    // Generate coins pos
    std::vector<int> columnIndices = { 0, 1, 2 };
    std::shuffle(columnIndices.begin(), columnIndices.end(), rng);

    int maxColumns = std::min(3, static_cast<int>(maxCoinsAvailable / 4)); // Min 4 coins per column
    maxColumns = std::max(1, maxColumns); // Ensure 1 column min

    std::vector<Vector3> newRewardOffsets;
    int coinColumnCount = 1 + rand() % maxColumns;

    for (int c = 0; c < coinColumnCount && coinsUsed < maxCoinsAvailable; c++)
    {
        float coinX = columns[columnIndices[c]];
        int remainingCoins = static_cast<int>(maxCoinsAvailable - coinsUsed);
        int coinCount = std::min(4 + rand() % 3, remainingCoins); // 4–6, limited by remaining coins

        bool fromTop = rand() % 2 == 0;

        int startZ = fromTop ? -4 : 4;
        int direction = fromTop ? 1 : -1;

        for (int i = 0; i < coinCount; i++)
        {
            int z = startZ + i * direction;
            if (z < -4 || z > 4) break;

            newRewardOffsets.emplace_back(coinX, 0.2f, static_cast<float>(z));
            coinsUsed++;
        }
    }

    std::vector<Vector3> newObstacleOffsets;
    int maxObstaclesAvailable = part.obstacles.size();
    int assignedObstaclePoses = 0;
    // Generate box poses
    std::vector<int> zRows;
    for (int z = -4; z <= 4; z += 2) // Add some space btw boxes in case appear same column
    {
        zRows.push_back(z);
    }

    std::shuffle(zRows.begin(), zRows.end(), rng);

    int obstaclesPlaced = 0;

    for (int z : zRows)
    {
        std::shuffle(columnIndices.begin(), columnIndices.end(), rng); // Mezclamos columnas

        int maxBoxesPerRow = 2; // max 2 per row to leave free path
        int placedInRow = 0;

        for (int i = 0; i < columns.size() && obstaclesPlaced < maxObstaclesAvailable && placedInRow < maxBoxesPerRow; i++)
        {
            float x = columns[columnIndices[i]];

            bool overlapsCoin = std::any_of(
                newRewardOffsets.begin(), newRewardOffsets.end(),
                [&](const Vector3& coinOffset) {
                    return coinOffset.x == x && static_cast<int>(coinOffset.z) == z;
                });

            if (overlapsCoin)
            {
                continue;
            }

            newObstacleOffsets.emplace_back(x, 0.0f, static_cast<float>(z));
            obstaclesPlaced++;
            placedInRow++;
        }
    }

    const Vector3& basePos = part.floorPart.originalPos;

    // Boxes repos
    int i = 0;
    for (; i < part.obstacles.size(); i++)
    {
        if (i < newObstacleOffsets.size())
        {
            Vector3 newPos = basePos + newObstacleOffsets[i];
            part.obstacles[i].transformComponent.SetPosition(newPos);
            part.obstacles[i].originalPos = newPos;
            part.obstacles[i].entity.GetComponent<EnableComponent>().Enabled = true;
        }
        else
        {
            part.obstacles[i].entity.GetComponent<EnableComponent>().Enabled = false;
        }
    }

    // Coin repos
    i = 0;
    for (; i < part.rewards.size(); i++)
    {
        if (i < newRewardOffsets.size())
        {
            Vector3 newPos = basePos + newRewardOffsets[i];
            part.rewards[i].transformComponent.SetPosition(newPos);
            part.rewards[i].originalPos = newPos;
            part.rewards[i].entity.GetComponent<EnableComponent>().Enabled = true;
        }
        else
        {
            part.rewards[i].entity.GetComponent<EnableComponent>().Enabled = false;
        }
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