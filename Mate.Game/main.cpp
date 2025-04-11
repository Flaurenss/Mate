// Mate.Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Mate.h>

void ManagePlayerInput(TransformComponent& transform, float deltaTime);

int main()
{
    auto avocadoModel = "./Assets/Avocado/Avocado.gltf";
    auto playerModel = "./Assets/Player/character.glb";
    Engine* engine = new Engine();
    ECS& registry = engine->GetRegistry();
    
    auto camera = registry.CreateEntity();
    camera.AddComponent<TransformComponent>();
    camera.AddComponent<CameraComponent>();

    auto floorMesh = PrimitivesHelper::CreatePlane();
    auto floor = registry.CreateEntity();
    floor.AddComponent<TransformComponent>();
    floor.AddComponent<MeshComponent>(floorMesh);
    TransformComponent& floorTrans = floor.GetComponent<TransformComponent>();
    floorTrans.Scale(Vector3(5, 1, 5));

    auto player = registry.CreateEntity();
    player.AddComponent<TransformComponent>();
    player.AddComponent<MeshComponent>(playerModel);
    TransformComponent& playerTransform = player.GetComponent<TransformComponent>();
    playerTransform.scale = (Vector3(0.1f, 0.1f, 0.1f));
    playerTransform.SetPosition(Vector3(0, 0.1f, 0));

    auto avocado = registry.CreateEntity();
    avocado.AddComponent<TransformComponent>();
    avocado.AddComponent<MeshComponent>(avocadoModel);
    TransformComponent& boxTransform = avocado.GetComponent<TransformComponent>();
    boxTransform.SetPosition(Vector3(0, 0.01f, 0));
    boxTransform.Scale(Vector3(1, 1, 1));
    
    float rotationSpeedDegrees = 90.0f;
    float movementSpeedUnits = 0.1f;
    while (engine->IsRunning())
    {
        ManagePlayerInput(playerTransform, engine->DeltaTime);

        float deltaTime = engine->DeltaTime;
        
        float deltaRotationY = rotationSpeedDegrees * deltaTime;
        boxTransform.Rotate(Vector3(0, deltaRotationY, 0));
        
        Vector3 currentPosition = boxTransform.position;
        float deltaMovementX = movementSpeedUnits * deltaTime;
        //boxTransform.Translate(Vector3(deltaMovementX, 0.0f, 0.0f));

        engine->Update();
        engine->Render();
    }
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
