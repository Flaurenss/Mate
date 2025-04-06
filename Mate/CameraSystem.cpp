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
		Camera& camera = cameraComponent.GetCamera();
		Matrix4 view = camera.GetViewMatrix();
		 
		Matrix4 projection = Matrix4();
		projection.perspective(45.0f, (float)1920 / (float)1080, 0.1f, 100.0f);

		shader.SetMat4("view", camera.GetViewMatrix());
		shader.SetMat4("projection", projection);
	}
}

void CameraSystem::SetResolution(int w, int h)
{
	width = w;
	height = h;
}
