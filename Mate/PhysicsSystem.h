#pragma once
#include "ECS.h"
#include "PhysicsEngine.h"

class PhysicsSystem : public System
{
public:
	PhysicsSystem();

	void Update(float deltaTime);
private:
	std::unique_ptr<PhysicsEngine> phyEngine;

	void RegisterBody(Entity& entity);
	void CallOnCollisionData(int selfId, int otherId);
};