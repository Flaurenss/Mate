#pragma once
#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>

class Entity;
struct PhysicsData
{
	JPH::BodyID bodyId;
	std::function<void(Entity otherEntity)> OnCollide;
	Entity Entity;
};