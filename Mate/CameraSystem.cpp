#include "CameraSystem.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "Logger.h"

CameraSystem::CameraSystem(Shader& shader) : shader(shader)
{
	RequireComponent<TransformComponent>();
	RequireComponent<CameraComponent>();
}

void CameraSystem::Start()
{
}

void CameraSystem::Update()
{
    for (Entity& entity : GetEntities())
    {
        shader.Use();

        TransformComponent& cameraTransform = entity.GetComponent<TransformComponent>();
        CameraComponent& cameraComponent = entity.GetComponent<CameraComponent>();

        Matrix4 projection;
        projection.perspective(
            cameraComponent.Fov,
            static_cast<float>(width) / static_cast<float>(height),
            cameraComponent.Near,
            cameraComponent.Far
        );

        Vector3 position = cameraTransform.Position;
        Vector3 forward = cameraTransform.GetForward().normalize();
        Vector3 right = Vector3::cross(forward, Vector3::Up).normalize();
        Vector3 up = Vector3::cross(right, forward).normalize();

        Matrix4 view = Matrix4::lookAt(position, position + forward, up);

        shader.SetMat4("view", view);
        shader.SetMat4("projection", projection);
    }
}

void CameraSystem::End()
{
}

void CameraSystem::SetResolution(int w, int h)
{
	width = w;
	height = h;
}
