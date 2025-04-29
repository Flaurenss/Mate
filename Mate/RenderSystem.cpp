#include "RenderSystem.h"
#include "TransformComponent.h"
#include "EnableComponent.h"
#include "MeshComponent.h"
#include "DebugDraw.h"

RenderSystem::RenderSystem(Shader& sh) : shader(sh)
{
	RequireComponent<TransformComponent>();
	RequireComponent<MeshComponent>();
}

void RenderSystem::Update()
{
	for (Entity& entity : GetEntities())
	{
		if (entity.HasComponent<EnableComponent>() &&
			!entity.GetComponent<EnableComponent>().Enabled)
		{
			continue;
		}

		MeshComponent& meshComponent = entity.GetComponent<MeshComponent>();
		TransformComponent& transformComponent = entity.GetComponent<TransformComponent>();
		shader.Use();
		Matrix4 modelTransform = transformComponent.GetTransform();
		shader.SetMat4("model", modelTransform);
		meshComponent.GetModel().Draw(shader);

		// Draw bounding box
		//const Vector3& extent = meshComponent.GetExtents();
		//DebugDraw::DrawAABB((extent/2), modelTransform, shader);
		// Draw world axis
		//DebugDraw::DrawWorldAxes(shader);
	}
}
