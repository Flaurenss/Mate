#include "EndlessRunner.h"
#include "GameAssets.h"
#include <random>


EndlessRunner::EndlessRunner(MateEngine& engine) :
	engine(engine)
{
	Setup();
}

void EndlessRunner::Setup()
{
    CreateCamera();
    engine.SetSkybox("skybox", {
        "./Assets/Skybox/right.jpg",
        "./Assets/Skybox/left.jpg",
        "./Assets/Skybox/top.jpg",
        "./Assets/Skybox/bottom.jpg",
        "./Assets/Skybox/front.jpg",
        "./Assets/Skybox/back.jpg" });

    AssetManager::GetInstance().LoadAudioClip("bg", "./Assets/Audio/bg.mp3");
    AssetManager::GetInstance().LoadAudioClip("coin", "./Assets/Audio/coin.wav");
    AssetManager::GetInstance().LoadAudioClip("hit", "./Assets/Audio/hitCustom.wav");

    // Audio settings
    auto bg = engine.CreateEntity();
    auto& bgComp = bg.AddComponent<AudioComponent>("bg", true, true);
    bgComp.SetVolume(0.5f);
    bgComp.SetIsUnique(true);
    bgComp.SetIsLoop(true);

    auto coinAudio = engine.CreateEntity();
    auto& coinComp = coinAudio.AddComponent<AudioComponent>("coin", false, false);
    coinComp.SetVolume(0.3f);

    auto hitAudio = engine.CreateEntity();
    auto& hitComp = hitAudio.AddComponent<AudioComponent>("hit", false, false);

	player = std::make_unique<Entity>(
        GameAssets::CreatePlayer(engine, Vector3::Up * 0.3f, Vector3(0, -180, 0), Vector3(0.5f))
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
            coinComp.Play();
            IncreaseDifficulty();
        }
        else if (tag == OBSTACLE_TAG)
        {
            gameOver = true;
            isPaused = true;
            reset = true;
            hitComp.Play();
        }
	};

    CreateEnvironment();
}

void EndlessRunner::Start()
{
    Logger::Log("=== Game Instructions ===");
    Logger::Log("Use 'A' and 'D' to move the character left and right.");
    Logger::Log("Press 'P' to pause or resume the game.");
    Logger::Log("Avoid obstacles and collect rewards!");
    Logger::Log("==========================");
}

void EndlessRunner::Update(float deltaTime)
{
    if (Input::GetKeyDown(KeyCode::P) && !reset)
    {
        isPaused = !isPaused;
    }

    if (Input::GetKeyDown(KeyCode::Escape))
    {
        isGameRunning = false;
    }

    if (!isPaused)
    {
        ProcessPlayerInput();
    }
    player->GetComponent<AnimationComponent>().CurrentAnimationIndex = isPaused ? 1 : 2;
    accumulator += deltaTime;
}

void EndlessRunner::FixedUpdate(float fixedDeltaTime)
{
    if (isPaused)
    {
        PauseGame();
    }
    else
    {
        MovePlayer(fixedDeltaTime);
        MoveEnvironment(fixedDeltaTime);
    }
    if (reset)
    {
        ResetAll(fixedDeltaTime);
    }
}

bool EndlessRunner::IsGameRunning() const
{
    return isGameRunning;
}

Entity EndlessRunner::CreateCamera()
{
    auto camera = engine.CreateEntity();
    camera.AddComponent<TransformComponent>(Vector3(0, 1.5f, 2.5f), Vector3(), Vector3(1));
    camera.AddComponent<CameraComponent>();
    TransformComponent& trans = camera.GetComponent<TransformComponent>();
    trans.LookAt(Vector3(0, 0.65f, 0), trans.GetUp());
    return camera;
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

void EndlessRunner::CreateEnvironment()
{
    for (auto i = 0; i < ROAD_LENGHT; i++)
    {
        auto envPart = CreateEnvironmentPart(i);
        RedoEnvironmentPart(envPart, true);
        environmentParts.push_back(envPart);
    }
}

EnvironmentPart EndlessRunner::CreateEnvironmentPart(int i)
{
    auto roadModelPath = "./Assets/Environment/Road/road-straight.glb";
    AssetManager::GetInstance().LoadModel("road", roadModelPath);
    auto road = engine.CreateEntity();
    auto roadPos = Vector3(0, 0, 0 + (-i * 10));
    auto& roadTrans = road.AddComponent<TransformComponent>(roadPos, Vector3::Zero, Vector3(5, 1, 10));
    road.AddComponent<PhysicsComponent>(MotionType::KINEMATIC, PhysicLayer::NON_COLLIDING);
    road.AddComponent<MeshComponent>("road");
    Part floorPart{ road, roadPos, roadTrans, EnvironmentType::Floor };

    std::vector<Part> rewards;
    std::vector<Part> obstacles;

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

    EnvironmentPart envPart{ i == 0 , floorPart, obstacles, rewards };
    return envPart;
}

Part EndlessRunner::CreateObstacle(Vector3 pos)
{
    Vector3 obstaclePos = pos + Vector3::Up * 2.0f;
    auto obstacle = GameAssets::CreateObstacle(engine, obstaclePos);
    obstacle.AddComponent<EnableComponent>().Enabled = false;
    auto& phyComponent = obstacle.AddComponent<PhysicsComponent>(MotionType::KINEMATIC, PhysicLayer::NON_MOVING);
    phyComponent.SetIsSensor(true);
    phyComponent.SetTag(OBSTACLE_TAG);
    Part obstaclePart{ obstacle, obstaclePos, obstacle.GetComponent<TransformComponent>(), EnvironmentType::Obstacle};
    return obstaclePart;
}

Part EndlessRunner::CreateReward(Vector3 pos)
{
    Vector3 rewardPos = pos + Vector3::Up * 0.2f;
    auto reward = GameAssets::CreateReward(engine, rewardPos);
    reward.AddComponent<EnableComponent>().Enabled = false;
    auto& phy = reward.AddComponent<PhysicsComponent>(MotionType::KINEMATIC, PhysicLayer::NON_MOVING);
    phy.SetIsSensor(true);
    phy.SetTag(REWARD_TAG);
    Part rewardPart{ reward, rewardPos, reward.GetComponent<TransformComponent>(), EnvironmentType::Reward};
    return rewardPart;
}

void EndlessRunner::ResetEnvironmentPart(EnvironmentPart& part, const Vector3& newFloorPos)
{
    part.floorPart.entity.GetComponent<PhysicsComponent>().MoveKinematic(newFloorPos);
    part.floorPart.originalPos = newFloorPos;

    RedoEnvironmentPart(part);
}

void EndlessRunner::RedoEnvironmentPart(EnvironmentPart& part, bool init)
{
    if (part.firstBlock && init || (part.obstacles.empty() && part.rewards.empty()))
    {
        for (auto& box : part.obstacles)
        {
            box.entity.GetComponent<EnableComponent>().Enabled = false;
        }

        for (auto& coin : part.rewards)
        {
            coin.entity.GetComponent<EnableComponent>().Enabled = false;
        }

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
        auto& boxPhy = part.obstacles[i].entity.GetComponent<PhysicsComponent>();
        if (i < newObstacleOffsets.size())
        {
            Vector3 newPos = basePos + newObstacleOffsets[i];
            newPos.y = 0.2f;
            if (init)
            {
                part.obstacles[i].transformComponent.SetPosition(newPos);
            }
            else
            {
                boxPhy.MoveKinematic(newPos);
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
        auto& coinPhy = part.rewards[i].entity.GetComponent<PhysicsComponent>();
        if (i < newRewardOffsets.size())
        {
            Vector3 newPos = basePos + newRewardOffsets[i];
            if (init)
            {
                part.rewards[i].transformComponent.SetPosition(newPos);
            }
            else
            {
                coinPhy.MoveKinematic(newPos);
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

void EndlessRunner::MoveEnvironment(float fixedDeltaTime)
{
    const float blockLength = 10.0f;

    // Movement Params
    const float speed = fixedDeltaTime * environmentSpeed;
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

void EndlessRunner::IncreaseDifficulty()
{
    static int lastDifficultyLevel = 0;

    int currentLevel = points / 10;

    if (currentLevel > lastDifficultyLevel)
    {
        lastDifficultyLevel = currentLevel;

        environmentSpeed = std::min(environmentSpeed + 0.5f, maxEnvironmentSpeed);
    }
}

void EndlessRunner::PauseGame()
{
    for (auto& environmentPart : environmentParts)
    {
        auto posToMove = environmentPart.floorPart.transformComponent.Position;
        environmentPart.floorPart.entity.GetComponent<PhysicsComponent>().MoveKinematic(posToMove);

        for (auto& coll : environmentPart.obstacles)
        {
            auto posToMove = coll.transformComponent.Position;
            coll.entity.GetComponent<PhysicsComponent>().MoveKinematic(posToMove);
        }

        for (auto& reward : environmentPart.rewards)
        {
            auto posToMove = reward.transformComponent.Position;
            reward.entity.GetComponent<PhysicsComponent>().MoveKinematic(posToMove);
        }
    }
}

void EndlessRunner::ResetAll(float fixedDeltaTime)
{
    if (gameOver)
    {
        const float blockLength = 10.0f;
        Vector3 basePosition = Vector3::Zero;
        for (size_t i = 0; i < environmentParts.size(); ++i)
        {
            auto& part = environmentParts[i];
            Vector3 partPos = basePosition + Vector3::Forward * (i * blockLength);
            ResetEnvironmentPart(part, partPos);

            part.firstBlock = (i == 0);

            RedoEnvironmentPart(part, (i == 0));
        }
        gameOver = false;
        Logger::Log("End of the game with " + std::to_string(points) + " points.");
        points = 0;
    }

    player->GetComponent<PhysicsComponent>().MoveKinematic(playerStartPos);
    railState.targetX = 0.0f;
    railState.currentRail = 0;

    if (fixedDeltaTime > actualResetTime)
    {
        actualResetTime += fixedDeltaTime;
    }
    else
    {
        actualResetTime = 0;
        reset = false;
    }
}
