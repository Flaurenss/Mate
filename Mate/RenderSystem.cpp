#include "RenderSystem.h"
#include "TransformComponent.h"
#include "MeshComponent.h"

RenderSystem::RenderSystem()
{
	RequireComponent<TransformComponent>();
	RequireComponent<MeshComponent>();
}

void RenderSystem::Update()
{
	for (auto entity : GetEntities())
	{

	}
}
