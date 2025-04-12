#include "CameraSystem.h"
#include "TransformComponent.h"
#include "CameraComponent.h"

CameraSystem::CameraSystem(Shader& shader) : shader(shader)
{
	RequireComponent<TransformComponent>();
	RequireComponent<CameraComponent>();
}

void CameraSystem::Update()
{
	for (Entity& entity : GetEntities())
	{
		shader.Use();

		CameraComponent& cameraComponent = entity.GetComponent<CameraComponent>();
		 
		Matrix4 projection = Matrix4();
		projection.perspective(cameraComponent.Fov, (float)width / (float)height, cameraComponent.Near, cameraComponent.Far);

		shader.SetMat4("view", cameraComponent.GetViewMatrix());
		shader.SetMat4("projection", projection);
	}
}

void CameraSystem::SetResolution(int w, int h)
{
	width = w;
	height = h;
}
