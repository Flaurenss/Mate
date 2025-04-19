// Mate.Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Mate.h>

float yaw = -90.0f;
float pitch = -20.0f;
float lastX = 0;
float lastY = 0;
bool firstMouse = true;

void CreateFloor(ECS& ecs);
Entity CreateCamera(ECS& ecs);
TransformComponent& CreatePlayer(ECS& ecs);
TransformComponent& CreateMisc(ECS& ecs);
void CreateMovableMisc(ECS& registry);
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
    //CreateFloor(ecs);
    CreateMovableMisc(ecs);
    TransformComponent& playerTransform = CreatePlayer(ecs);
    //TransformComponent& avcTransform = CreateMisc(ecs);

    
    float rotationSpeedDegrees = 90.0f;
    float movementSpeedUnits = 0.1f;
    Vector3 originalPos = playerTransform.Position;
    while (engine->IsRunning())
    {
        float deltaTime = engine->DeltaTime;
        //ManagePlayerInputRails(playerTransform, deltaTime, originalPos);
        ManageFreeCamera(cameraComponent, cameraTransform, deltaTime);

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
    camera.AddComponent<TransformComponent>(Vector3(0, 2.0f, 2.5f), Vector3(1, 1, 1), Vector3());
    camera.AddComponent<CameraComponent>();
    CameraComponent& cameraComponent = camera.GetComponent<CameraComponent>();
    cameraComponent.SetForward(Vector3(0, 0, 0));
    return camera;
}

TransformComponent& CreatePlayer(ECS& ecs)
{
    auto playerModel = "./Assets/Player/character.glb";
    auto player = ecs.CreateEntity();
    player.AddComponent<TransformComponent>();
    player.AddComponent<MeshComponent>(playerModel);
    TransformComponent& playerTransform = player.GetComponent<TransformComponent>();
    playerTransform.SetPosition(Vector3(0, 0.01f, 0));
    playerTransform.DoScale(0.5f);
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

void CreateMovableMisc(ECS& ecs)
{
    auto coinModel = "./Assets/Environment/Misc/coin.glb";
    auto boxModel = "./Assets/Environment/Misc/crate-color.glb";
    auto roadModel = "./Assets/Environment/Road/road-straight.glb";

    auto coin = ecs.CreateEntity();
    coin.AddComponent<TransformComponent>(Vector3(-0.2f, 0.2f, -1), Vector3(0, -90, 0), Vector3::One);
    coin.AddComponent<MeshComponent>(coinModel);

    auto box = ecs.CreateEntity();
    box.AddComponent<TransformComponent>(Vector3(0.2f, 0, 1));
    box.AddComponent<MeshComponent>(boxModel);

    auto road = ecs.CreateEntity();
    road.AddComponent<TransformComponent>(Vector3::Zero, Vector3::Zero, Vector3(5, 5, 5));
    road.AddComponent<MeshComponent>(roadModel);
}

void ManagePlayerInputRails(TransformComponent& transform, float deltaTime, Vector3 originalPos)
{
    float space = 1.5f;
    float maxRight = originalPos.x + space;
    float maxLeft = originalPos.x - space;

    // Mover a la derecha
    if (Input::GetKeyDown(KeyCode::D))
    {
        float newX = transform.Position.x + space;
        if (newX <= maxRight)
        {
            transform.Position.x = newX;
        }
    }

    // Mover a la izquierda
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
    float speed = deltaTime * 2.5f;
    Vector3 direction = Vector3::Zero;
    
    // User keyboard input
    if (Input::GetKey(KeyCode::W))
    {
        direction += cameraComponent.GetForward();
    }
    if (Input::GetKey(KeyCode::A))
    {
        direction -= cameraComponent.GetRight();
    }
    if (Input::GetKey(KeyCode::S))
    {
        direction -= cameraComponent.GetForward();
    }
    if (Input::GetKey(KeyCode::D))
    {
        direction += cameraComponent.GetRight();
    }

    float xPos = Input::MousePosition.x;
    float yPos = Input::MousePosition.y;
    float sensitivity = 0.1f;
    // User mouse movement
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xoffset = xPos - lastX;
    float yoffset = lastY - yPos; // reversed since y-coordinates range from bottom to top
    lastX = xPos;
    lastY = yPos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 80.0f)
        pitch = 80.0f;
    if (pitch < -80.0f)
        pitch = -80.0f;

    Vector3 newForward;
    newForward.x = cosf(MathUtils::radians(yaw)) * cosf(MathUtils::radians(pitch));
    newForward.y = sinf(MathUtils::radians(pitch));
    newForward.z = sinf(MathUtils::radians(yaw)) * cosf(MathUtils::radians(pitch));

    newForward = newForward.normalize();
    auto newRight = Vector3::cross(newForward, Vector3::Up).normalize();
    auto newUp = Vector3::cross(newRight, newForward);

    cameraTransform.Translate(direction.normalize() * speed);
    cameraComponent.SetForward(newForward);
    cameraComponent.SetRight(newRight);
    cameraComponent.SetUp(newUp);
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

struct EnvironmentAsset
{
    std::vector<Entity> objects;
};

enum EnvironmentType
{
    Floor,
    Obstacle,
    Reward
};