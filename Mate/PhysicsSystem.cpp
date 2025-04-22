#include "PhysicsSystem.h"
#include "TransformComponent.h"


PhysicsSystem::PhysicsSystem()
{
	JPH::RegisterDefaultAllocator();
	RequireComponent<TransformComponent>();
}

void PhysicsSystem::Update()
{
	for (Entity& entity : GetEntities())
	{
		
	}
}
