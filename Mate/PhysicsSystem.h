#pragma once
#include "ECS.h"
#include "PhysicsEngine.h"

class PhysicsSystem : public System
{
public:
	PhysicsSystem();

	void Update(float fixedDeltaTime);
private:
	std::unique_ptr<PhysicsEngine> phyEngine;
};