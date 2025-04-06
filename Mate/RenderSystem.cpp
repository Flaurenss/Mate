#include "RenderSystem.h"
#include "TransformComponent.h"
#include "MeshComponent.h"

RenderSystem::RenderSystem()
{
	RequireComponent<TransformComponent>();
	RequireComponent<MeshComponent>();

	shader = new Shader("./vertexShader.shader", "./fragmentShader.shader");
}

void RenderSystem::Update()
{
	for (Entity& entity : GetEntities())
	{
		MeshComponent& meshComponent = entity.GetComponent<MeshComponent>();
		meshComponent.GetModel().Draw(*shader);
	}
}
