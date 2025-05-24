#include "CameraSystem.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "Logger.h"

#include <glad/glad.h>
#include "stb_image.h"

CameraSystem::CameraSystem()
{
	RequireComponent<TransformComponent>();
	RequireComponent<CameraComponent>();
}

void CameraSystem::Update(RenderContext& renderContext)
{
    for (Entity& entity : GetEntities())
    {
        TransformComponent& cameraTransform = entity.GetComponent<TransformComponent>();
        CameraComponent& cameraComponent = entity.GetComponent<CameraComponent>();

        Matrix4 projection;
        projection.perspective(
            cameraComponent.Fov,
            static_cast<float>(renderContext.Width) / static_cast<float>(renderContext.Height),
            cameraComponent.Near,
            cameraComponent.Far
        );

        Vector3 position = cameraTransform.Position;
        Vector3 forward = cameraTransform.GetForward().normalize();
        Vector3 right = Vector3::cross(forward, Vector3::Up).normalize();
        Vector3 up = Vector3::cross(right, forward).normalize();

        Matrix4 view = Matrix4::lookAt(position, position + forward, up);

        renderContext.View = view;
        renderContext.Projection = projection;
    }
}
