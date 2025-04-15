// Mate.Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Mate.h>

void CreateFloor(ECS& registry);
void CreateCamera(ECS& ecs);
TransformComponent& CreatePlayer(ECS& ecs);
TransformComponent& CreateMisc(ECS& registry);
void ManagePlayerInputRails(TransformComponent& transform, float deltaTime, Vector3 originalPos);
void ManagePlayerInput(TransformComponent& transform, float deltaTime);

int main()
{
    Engine* engine = new Engine();
    ECS& ecs = engine->GetRegistry();
    
    CreateCamera(ecs);
    CreateFloor(ecs);
    TransformComponent& playerTransform = CreatePlayer(ecs);
    TransformComponent& avcTransform = CreateMisc(ecs);

    
    float rotationSpeedDegrees = 90.0f;
    float movementSpeedUnits = 0.1f;
    Vector3 originalPos = playerTransform.Position;
    while (engine->IsRunning())
    {
        ManagePlayerInputRails(playerTransform, engine->DeltaTime, originalPos);

        float deltaTime = engine->DeltaTime;
        
        float deltaRotationY = rotationSpeedDegrees * deltaTime;
        avcTransform.Rotate(Vector3(0, deltaRotationY, 0));
        
        Vector3 currentPosition = avcTransform.Position;
        float deltaMovementX = movementSpeedUnits * deltaTime;
        //boxTransform.Translate(Vector3(deltaMovementX, 0.0f, 0.0f));

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

void CreateCamera(ECS& ecs)
{
    auto camera = ecs.CreateEntity();
    camera.AddComponent<TransformComponent>(Vector3(0, 2.0f, 2.5f), Vector3(1, 1, 1), Vector3());
    camera.AddComponent<CameraComponent>();
    CameraComponent& cameraComponent = camera.GetComponent<CameraComponent>();
    cameraComponent.SetForward(Vector3(0, 0, 0));
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

void ManagePlayerInputRails(TransformComponent& transform, float deltaTime, Vector3 originalPos)
{
    float space = 2.0f;
    float maxRight = originalPos.x + space;
    float maxLeft = originalPos.x - space;

    // Mover a la derecha
    if (Input::GetKeyDown(KeyCode::D))
    {
        float newX = transform.Position.x + space;
        if (newX <= maxRight)
            transform.Position.x = newX;
    }

    // Mover a la izquierda
    if (Input::GetKeyDown(KeyCode::A))
    {
        float newX = transform.Position.x - space;
        if (newX >= maxLeft)
            transform.Position.x = newX;
    }
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
