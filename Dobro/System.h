#pragma once
#include <vector>

class Entity;

// Transforms Components from state A to state B
// Performs logic on components and entities
class System
{
public:
	virtual void Update() = 0;

	void RequireComponent();
	std::vector<Entity*> GetEntities();
};