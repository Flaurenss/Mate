#include "PhysicsSystem.h"
#include "TransformComponent.h"
#include "EnableComponent.h"

PhysicsSystem::PhysicsSystem()
{
	phyEngine = std::make_unique<PhysicsEngine>();
	RequireComponent<TransformComponent>();
}

void PhysicsSystem::Update()
{
	for (Entity& entity : GetEntities())
	{
		if (entity.HasComponent<EnableComponent>()
			&& entity.GetComponent<EnableComponent>().Enabled)
		{

		}
	}
}