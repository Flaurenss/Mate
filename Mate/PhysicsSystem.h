#pragma once
#include "ECS.h"
#include <Jolt/Jolt.h>
#include "PhysicsEngine.h"

class PhysicsSystem : public System
{
public:
	PhysicsSystem();

	virtual void Update();
private:
	std::unique_ptr<PhysicsEngine> phyEngine;
};

