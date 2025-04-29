#pragma once
#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include "PhysicsComponent.h"

struct PhysicsData
{
	JPH::BodyID BodyId;
	PhysicsComponent& Component;

	PhysicsData(JPH::BodyID bodyId, PhysicsComponent& physicsComponent);
};