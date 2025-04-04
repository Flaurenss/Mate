#pragma once
#include <vector>
#include "Entity.h"

// TODO: rethink name, something like EntityManager?
class ECS
{
public:
	void AddEntity(Entity* entity);
	void Update(float deltaTime);
	void Render();
private:
	std::vector<Entity*> entities;
};