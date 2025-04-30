#include "EndlessRunner.h"
#include "GameAssets.h"
#include <random>


EndlessRunner::EndlessRunner(ECS& ecs) :
	ecs(ecs),
	modelImporter(ModelImporter())
{
	Setup();
}

void EndlessRunner::Setup()
{
	player = std::make_unique<Entity>(
		GameAssets::CreatePlayer(ecs, modelImporter, Vector3::Up * 0.2f)
	);

    playerStartPos = player.get()->GetComponent<TransformComponent>().Position;
    railState.targetX = playerStartPos.x;
    railState.currentRail = 0;

	player.get()->GetComponent<PhysicsComponent>().OnCollide = [&](Entity otherEntity)
	{
        auto& otherPhysics = otherEntity.GetComponent<PhysicsComponent>();
        auto tag = otherPhysics.GetTag();
        if (tag == REWARD_TAG)
        {
            otherEntity.GetComponent<EnableComponent>().Enabled = false;
            points++;
        }
        else if (tag == OBSTACLE_TAG)
        {
            runGame = false;
        }
	};

    CreateEnvironment();
}

void EndlessRunner::CreateEnvironment()
{
    for (auto i = 0; i < ROAD_LENGHT; i++)
    {
        auto envPart = CreateEnvironmentPart(ecs, i);
        RedoEnvironmentPart(envPart, true);
        environmentParts.push_back(envPart);
    }
}

EnvironmentPart EndlessRunner::CreateEnvironmentPart(ECS& ecs, int i)
{
    auto roadModelPath = "./Assets/Environment/Road/road-straight.glb";
    auto roadMeshes = modelImporter.Load(roadModelPath);
    auto road = ecs.CreateEntity();
    auto roadPos = Vector3(0, 0, 0 + (-i * 10));
    auto& roadTrans = road.AddComponent<TransformComponent>(roadPos, Vector3::Zero, Vector3(5, 1, 10));
    road.AddComponent<PhysicsComponent>(MotionType::KINEMATIC, PhysicLayer::NON_MOVING);
    road.AddComponent<MeshComponent>(roadMeshes);
    //road.AddComponent<PhysicsComponent>();
    Part floorPart{ road, roadPos, roadTrans, EnvironmentType::Floor };

    std::vector<Part> rewards;
    std::vector<Part> obstacles;
    if (i != 0)
    {
        for (auto i = 0; i < START_OBSTACLES; i++)
        {
            auto boxPart = CreateObstacle(roadPos);
            obstacles.push_back(boxPart);
        }

        for (auto i = 0; i < START_COINS; i++)
        {
            auto coinPart = CreateReward(roadPos);
            rewards.push_back(coinPart);
        }
    }

    EnvironmentPart envPart{ floorPart, obstacles, rewards };
    return envPart;
}

void EndlessRunner::RedoEnvironmentPart(EnvironmentPart& part, bool init)
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

void EndlessRunner::ResetEnvironmentPart(EnvironmentPart& part, const Vector3& newFloorPos)
{
    part.floorPart.entity.GetComponent<PhysicsComponent>().MoveKinematic(newFloorPos);
    part.floorPart.originalPos = newFloorPos;

    RedoEnvironmentPart(part);
}

Part EndlessRunner::CreateObstacle(Vector3 pos)
{
    auto obstacle = GameAssets::CreateObstacle(ecs, modelImporter, pos);
    obstacle.AddComponent<EnableComponent>().Enabled = false;
    auto& phyComponent = obstacle.AddComponent<PhysicsComponent>(MotionType::KINEMATIC, PhysicLayer::NON_MOVING);
    phyComponent.SetIsSensor(true);
    phyComponent.SetTag(OBSTACLE_TAG);
    Part obstaclePart{ obstacle, pos, obstacle.GetComponent<TransformComponent>(), EnvironmentType::Obstacle};
    return obstaclePart;
}

Part EndlessRunner::CreateReward(Vector3 pos)
{
    auto reward = GameAssets::CreateReward(ecs, modelImporter, pos);
    reward.AddComponent<EnableComponent>().Enabled = false;
    auto& phy = reward.AddComponent<PhysicsComponent>(MotionType::KINEMATIC, PhysicLayer::NON_MOVING);
    phy.SetIsSensor(true);
    phy.SetTag(REWARD_TAG);
    Part rewardPart{ reward, pos, reward.GetComponent<TransformComponent>(), EnvironmentType::Reward};
    return rewardPart;
}

void EndlessRunner::Update(float deltaTime)
{
    if (runGame)
    {
       ProcessPlayerInput();
    }

    accumulator += deltaTime;
}

void EndlessRunner::FixedUpdate()
{
    MovePlayer(fixedDeltaTime);
    MoveEnvironment(fixedDeltaTime);
}

void EndlessRunner::ProcessPlayerInput()
{
    if (Input::GetKeyDown(KeyCode::D) && railState.currentRail < 1)
    {
        railState.currentRail++;
        railState.targetX = railState.currentRail * playerRailSpace;
    }

    if (Input::GetKeyDown(KeyCode::A) && railState.currentRail > -1)
    {
        railState.currentRail--;
        railState.targetX = railState.currentRail * playerRailSpace;
    }
}

void EndlessRunner::MovePlayer(float fixedDeltaTime)
{
    const float speed = 10.0f;
    const float epsilon = 0.01f;

    auto& transform = player->GetComponent<TransformComponent>();
    auto& physicsComponent = player->GetComponent<PhysicsComponent>();
    float currentX = transform.Position.x;
    float deltaX = railState.targetX - currentX;

    if (std::abs(deltaX) > epsilon)
    {
        float direction = (deltaX > 0) ? 1.0f : -1.0f;
        float move = speed * fixedDeltaTime;

        if (std::abs(deltaX) <= move)
        {
            physicsComponent.MoveKinematic(Vector3(railState.targetX, transform.Position.y, transform.Position.z));
        }
        else
        {
            Vector3 moveVec = Vector3::Right * move * direction;
            physicsComponent.MoveKinematic(transform.Position + moveVec);
        }
    }
    else
    {
        physicsComponent.MoveKinematic(Vector3(railState.targetX, transform.Position.y, transform.Position.z));
    }
}

void EndlessRunner::MoveEnvironment(float fixedDeltaTime)
{
    const float blockLength = 10.0f;

    // Movement Params
    const float speed = fixedDeltaTime * 7.5f;
    const float rotationSpeed = fixedDeltaTime * 200;
    Vector3 movement = -Vector3::Forward * speed;

    // Mover all assets on forward axis
    for (auto& environmentPart : environmentParts)
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
    auto& firstBlock = environmentParts.front();
    TransformComponent& firstFloorTrans = firstBlock.GetFloorTransform();
    Vector3 lastFloorPosition = environmentParts.back().GetFloorTransform().Position;
    if (firstFloorTrans.Position.z > 10.0f)
    {
        Vector3 newBasePos = lastFloorPosition - Vector3(0, 0, blockLength - movement.length());
        ResetEnvironmentPart(firstBlock, newBasePos);

        environmentParts.push_back(firstBlock);
        environmentParts.pop_front();
    }
}

bool EndlessRunner::IsGameRunning()
{
    return runGame;
}

void EndlessRunner::SetGameRunning(bool status)
{
    runGame = status;
}
