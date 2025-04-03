#pragma once
#include <vector>

class Component;

// Represented by IDs
// Populates scene
class Entity
{
public:
	void AddComponent(Component* component);
	void GetComponent();
	void Update(float deltaTime);
	void Render();

private:
	int id;
	std::vector<Component*> components;
};
