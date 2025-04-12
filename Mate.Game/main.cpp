// Mate.Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Mate.h>

void CreateFloor(ECS& registry);
void CreateCamera(ECS& ecs);
TransformComponent& CreatePlayer(ECS& ecs);
TransformComponent& CreateMisc(ECS& registry);
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
    while (engine->IsRunning())
    {
        ManagePlayerInput(playerTransform, engine->DeltaTime);

        float deltaTime = engine->DeltaTime;
        
        float deltaRotationY = rotationSpeedDegrees * deltaTime;
        avcTransform.Rotate(Vector3(0, deltaRotationY, 0));
        
        Vector3 currentPosition = avcTransform.position;
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
    floorTrans.Scale(Vector3(5, 1, 5));
}

void CreateCamera(ECS& ecs)
{
    auto camera = ecs.CreateEntity();
    camera.AddComponent<TransformComponent>();
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
    playerTransform.Scale(0.5f);
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
    avcTransform.Scale(2);
    return avcTransform;
}

void ManagePlayerInput(TransformComponent& transform, float deltaTime)
{
    auto velocity = deltaTime * 1.1f;
    if (Input::GetKey(KeyCode::W))
    {
        transform.Translate(Vector3::Forward * velocity);
    }

    if (Input::GetKey(KeyCode::S))
    {
        transform.Translate(Vector3::Back * velocity);
    }

    if (Input::GetKey(KeyCode::D))
    {
        transform.Translate(Vector3::Right * velocity);
    }

    if (Input::GetKey(KeyCode::A))
    {
        transform.Translate(Vector3::Left * velocity);
    }
}
