#pragma once
#include <vector>
#include "Entity.h"

// TODO: rethink name, something like EntityManager?
class ECS
{
public:
	void CreateEntity();
	void DestroyEntity();
	void AddSystem();
	void RemoveComponent();

	void AddEntity(Entity* entity);
	void Update(float deltaTime);
	void Render();
private:
	int entities;
	//std::vector<Entity*> entities;
};

// Pool component concept:
//  [0]....[1] [2] [3]
//componentID
//   |
//   v
//  [0] ->EntityId n0 
//  [1] -> EntityId n1