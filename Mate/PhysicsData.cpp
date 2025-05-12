#include "PhysicsData.h"
#include "Logger.h"

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
		Logger::Err("Entity does not contain a PhysicsComponent.");
	}
}
