#include "RenderSystem.h"
#include "TransformComponent.h"
#include "MeshComponent.h"

RenderSystem::RenderSystem(Shader& sh) : shader(sh)
{
	RequireComponent<TransformComponent>();
	RequireComponent<MeshComponent>();
}

void RenderSystem::Update()
{
	for (Entity& entity : GetEntities())
	{
		MeshComponent& meshComponent = entity.GetComponent<MeshComponent>();
		TransformComponent& transformComponent = entity.GetComponent<TransformComponent>();
		shader.Use();
		Matrix4 transform = transformComponent.GetTransform();
		shader.SetMat4("model", transform);
		meshComponent.GetModel().Draw(shader);
	}
}
