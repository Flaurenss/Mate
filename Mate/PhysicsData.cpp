#include "PhysicsData.h"

PhysicsData::PhysicsData(JPH::BodyID bodyId, PhysicsComponent& physicsComponent)
	: BodyId(bodyId), Component(physicsComponent)
{ }
