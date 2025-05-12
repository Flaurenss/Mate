// Mate.Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
// TODO: add pch with Mate.h
#include "EnvironmentAssets.h"
#include <deque>
#include <random>
#include "EngineDemo.h"
#include "GameAssets.h"
#include "Mate.h"

float yaw = 45.0f;
float pitch = 0;
float lastX = 0;
float lastY = 0;
float fixedDeltaTime = 1.0f / 60.0f;
bool firstMouse = true;
const int ROAD_LENGHT = 3;
const int START_COINS = 10;
const int START_OBSTACLES = 3;
const std::string REWARD_TAG = "COIN";
const std::string OBSTACLE_TAG = "BOX";

Entity CreateCamera(ECS& ecs);

std::deque<EnvironmentPart> CreateEnvironment(ECS& ecs);
EnvironmentPart CreateMovableMisc(ECS& registry, int i);
Part CreateCoin(Vector3 roadPos, float xOffset, float zOffset, ECS& ecs);
Part CreateBox(Vector3 roadPos, float xOffset, float zOffset, ECS& ecs);

void ManageMovableMisc(std::deque<EnvironmentPart>& assets, float deltaTime);
void ResetEnvironmentPart(EnvironmentPart& part, const Vector3& newFloorPos);
void RedoPart(EnvironmentPart& part, bool init = false);

void ManagePlayerInputRails(Entity player, PlayerRailState& state, float deltaTime);
void ProcessPlayerInputRails(Entity player, PlayerRailState& state, float playerOriginalX);

void ManageFreeCamera(CameraComponent& cameraComponent, TransformComponent& transformCamera, float deltaTime);
void ManagePlayerInput(Entity& entity, float deltaTime);

void GameLoop(ECS& ecs, Engine* engine);

int main()
{
    auto engine = std::make_unique<Engine>();
    ECS& ecs = engine->GetRegistry();

    GameLoop(ecs, engine.get());
}

void GameLoop(ECS& ecs, Engine* engine)
{
    bool runGame = false;
    int points = 0;

    auto camera = CreateCamera(ecs);
    TransformComponent& cameraTransform = camera.GetComponent<TransformComponent>();
    CameraComponent& cameraComponent = camera.GetComponent<CameraComponent>();
    //EngineDemo::PhysicsCubeDemo(ecs);
    Entity playerEntity = GameAssets::CreatePlayer(ecs, Vector3::Up * 0.3f, Vector3(0, -180, 0), Vector3(0.5f));

    playerEntity.GetComponent<PhysicsComponent>().OnCollide = [&](Entity otherEntity)
        {
            auto& otherPhysicsComponent = otherEntity.GetComponent<PhysicsComponent>();
            auto tag = otherPhysicsComponent.GetTag();
            if (tag == REWARD_TAG)
            {
                otherEntity.GetComponent<EnableComponent>().Enabled = false;
                points++;
            }
            else if (tag == OBSTACLE_TAG)
            {
                runGame = false;
                // TODO: put it in a PAUSE GAME method
                engine->SetSimulationTo(runGame);
            }
        };
    auto& animator = playerEntity.GetComponent<AnimationComponent>();

    auto environmentAssets = CreateEnvironment(ecs);
    auto& playerTransform = playerEntity.GetComponent<TransformComponent>();
    Vector3 originalPos = playerTransform.Position;
    PlayerRailState railState;
    railState.targetX = playerTransform.Position.x;
    float accumulator = 0;
    while (engine->IsRunning())
    {
        float deltaTime = engine->DeltaTime;
        if (Input::GetKeyDown(KeyCode::P))
        {
            runGame = !runGame;
            engine->SetSimulationTo(runGame);
        }

        animator.CurrentAnimationIndex = runGame ? 2 : 1;
        accumulator += deltaTime;
        if (runGame)
        {
            ProcessPlayerInputRails(playerEntity, railState, originalPos.x);
        }
        while (accumulator >= fixedDeltaTime)
        {
            if (runGame)
            {
                ManagePlayerInputRails(playerEntity, railState, fixedDeltaTime);
                ManageMovableMisc(environmentAssets, fixedDeltaTime);
                engine->PhysicsUpdate(fixedDeltaTime);
            }
            accumulator -= fixedDeltaTime;
        }

        //ManageFreeCamera(cameraComponent, cameraTransform, engine->DeltaTime);

        engine->Update();
    }
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

std::deque<EnvironmentPart> CreateEnvironment(ECS& ecs)
{
    std::deque<EnvironmentPart> environmentParts;
    for (auto i = 0; i < ROAD_LENGHT; i++)
    {
        auto envPart = CreateMovableMisc(ecs, i);
        RedoPart(envPart, true);
        environmentParts.push_back(envPart);
    }

    return environmentParts;
}

EnvironmentPart CreateMovableMisc(ECS& ecs, int i)
{
    auto roadModelPath = "./Assets/Environment/Road/road-straight.glb";
    AssetManager::GetInstance().LoadModel("road", roadModelPath);
    auto road = ecs.CreateEntity();
    auto roadPos = Vector3(0, 0, 0 + (-i * 10));
    auto& roadTrans = road.AddComponent<TransformComponent>(roadPos, Vector3::Zero, Vector3(5, 1, 10));
    road.AddComponent<PhysicsComponent>(MotionType::KINEMATIC, PhysicLayer::NON_MOVING);
    road.AddComponent<MeshComponent>("road");
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
    Vector3 coinPos = roadPos + Vector3::Up * 0.2f;
    auto reward = GameAssets::CreateReward(ecs, coinPos);
    reward.AddComponent<EnableComponent>().Enabled = false;
    auto& phy = reward.AddComponent<PhysicsComponent>(MotionType::KINEMATIC, PhysicLayer::NON_MOVING);
    phy.SetIsSensor(true);
    phy.SetTag(REWARD_TAG);
    Part rewardPart{ reward, coinPos, reward.GetComponent<TransformComponent>(), EnvironmentType::Reward };
    return rewardPart;
}

Part CreateBox(Vector3 roadPos, float xOffset, float zOffset, ECS& ecs)
{
    Vector3 boxPos = roadPos + Vector3::Up * 0.2f;
    auto obstacle = GameAssets::CreateObstacle(ecs, boxPos);
    obstacle.AddComponent<EnableComponent>().Enabled = false;
    auto& phyComponent = obstacle.AddComponent<PhysicsComponent>(MotionType::KINEMATIC, PhysicLayer::NON_MOVING);
    phyComponent.SetIsSensor(true);
    phyComponent.SetTag(OBSTACLE_TAG);
    Part obstaclePart{ obstacle, boxPos, obstacle.GetComponent<TransformComponent>(), EnvironmentType::Obstacle };
    return obstaclePart;
}

void ManageMovableMisc(std::deque<EnvironmentPart>& environmentPartsQueue, float deltaTime)
{
    const float blockLength = 10.0f;

    // Movement Params
    const float speed = deltaTime * 7.5f;
    const float rotationSpeed = deltaTime * 200;
    Vector3 movement = -Vector3::Forward * speed;

    // Mover all assets on forward axis
    for (auto& environmentPart : environmentPartsQueue)
    {
        auto posToMove = environmentPart.floorPart.transformComponent.Position + movement;
        environmentPart.floorPart.entity.GetComponent<PhysicsComponent>().MoveKinematic(posToMove);

        for (auto& coll : environmentPart.obstacles)
        {
            auto posToMove = coll.transformComponent.Position + movement;
            coll.entity.GetComponent<PhysicsComponent>().MoveKinematic(posToMove);
        }

        for (auto& reward : environmentPart.rewards)
        {
            auto posToMove = reward.transformComponent.Position + movement;
            reward.entity.GetComponent<PhysicsComponent>().MoveKinematic(posToMove);
            reward.transformComponent.Rotate(Vector3::Up * rotationSpeed);
        }
    }

    // Check if we need to reset
    auto& firstBlock = environmentPartsQueue.front();
    TransformComponent& firstFloorTrans = firstBlock.GetFloorTransform();
    Vector3 lastFloorPosition = environmentPartsQueue.back().GetFloorTransform().Position;
    if (firstFloorTrans.Position.z > 10.0f)
    {
        Vector3 newBasePos = lastFloorPosition - Vector3(0, 0, blockLength - movement.length());
        ResetEnvironmentPart(firstBlock, newBasePos);

        environmentPartsQueue.push_back(firstBlock);
        environmentPartsQueue.pop_front();
    }
}

void ResetEnvironmentPart(EnvironmentPart& part, const Vector3& newFloorPos)
{
    part.floorPart.entity.GetComponent<PhysicsComponent>().MoveKinematic(newFloorPos);
    part.floorPart.originalPos = newFloorPos;

    RedoPart(part);
}

// Used to reposition assets again when floor reset happens
void RedoPart(EnvironmentPart& part, bool init)
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
            newPos.y = 0.2f;
            //newPos *= Vector3::Up * 0.2f;
            if (init)
            {
                part.obstacles[i].transformComponent.SetPosition(newPos);
            }
            else
            {
                part.obstacles[i].entity.GetComponent<PhysicsComponent>().MoveKinematic(newPos);
            }
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
            if (init)
            {
                part.rewards[i].transformComponent.SetPosition(newPos);
            }
            else
            {
                part.rewards[i].entity.GetComponent<PhysicsComponent>().MoveKinematic(newPos);
            }
            part.rewards[i].originalPos = newPos;
            part.rewards[i].entity.GetComponent<EnableComponent>().Enabled = true;
        }
        else
        {
            part.rewards[i].entity.GetComponent<EnableComponent>().Enabled = false;
        }
    }
}

void ManagePlayerInputRails(Entity player, PlayerRailState& state, float deltaTime)
{
    const float speed = 10.0f;
    const float epsilon = 0.01f;

    auto& transform = player.GetComponent<TransformComponent>();
    auto& physicsComponent = player.GetComponent<PhysicsComponent>();
    float currentX = transform.Position.x;
    float deltaX = state.targetX - currentX;

    if (std::abs(deltaX) > epsilon)
    {
        float direction = (deltaX > 0) ? 1.0f : -1.0f;
        float move = speed * deltaTime;

        if (std::abs(deltaX) <= move)
        {
            physicsComponent.MoveKinematic(Vector3(state.targetX, transform.Position.y, transform.Position.z));
        }
        else
        {
            Vector3 moveVec = Vector3::Right * move * direction;
            physicsComponent.MoveKinematic(transform.Position + moveVec);
        }
    }
    else
    {
        physicsComponent.MoveKinematic(Vector3(state.targetX, transform.Position.y, transform.Position.z));
    }
}

void ProcessPlayerInputRails(Entity player, PlayerRailState& state, float playerOriginalX)
{
    const float space = 1.5f;

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

void ManagePlayerInput(Entity& entity, float deltaTime)
{
    auto velocity = deltaTime * 2.0f;
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

    auto& pos = entity.GetComponent<TransformComponent>().Position;
    auto& physics = entity.GetComponent<PhysicsComponent>();
    Vector3 posToMove = pos + direction.normalize() * velocity;
    physics.MoveKinematic(posToMove);
}