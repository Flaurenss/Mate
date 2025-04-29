#pragma once
#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include "Entity.h"
#include "PhysicsComponent.h"

struct PhysicsData
{
	JPH::BodyID BodyId;

	PhysicsData(JPH::BodyID bodyId, Entity entity);
	Entity GetEntity();
	PhysicsComponent& GetPhysicsComponent();

private:
	Entity entity;
};