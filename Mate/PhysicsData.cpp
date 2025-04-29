#include "PhysicsData.h"
#include <cassert>

PhysicsData::PhysicsData(JPH::BodyID bodyId, Entity entity)
	: BodyId(bodyId), entity(entity)
{ }

Entity PhysicsData::GetEntity()
{
	return entity;
}

PhysicsComponent& PhysicsData::GetPhysicsComponent()
{
	if (entity.HasComponent<PhysicsComponent>())
	{
		return entity.GetComponent<PhysicsComponent>();
	}
	else
	{
		assert(false && "PhysicsData::GetPhysicsComponent: Entity does not contain a PhysicsComponent.");
	}
}
