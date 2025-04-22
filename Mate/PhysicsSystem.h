#pragma once
#include "ECS.h"
#include <Jolt/Jolt.h>

class PhysicsSystem : public System
{
public:
	PhysicsSystem();

	virtual void Update();
};

