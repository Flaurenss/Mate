#pragma once
#include "Entity.h"
#include "PhysicsComponent.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>

struct PhysicsData
{
public:
	PhysicsData(JPH::BodyID bodyId, Entity entity);
	
	JPH::BodyID GetBodyId();
	Entity GetEntity();
	PhysicsComponent& GetPhysicsComponent();

private:
	JPH::BodyID bodyId;
	Entity entity;
};