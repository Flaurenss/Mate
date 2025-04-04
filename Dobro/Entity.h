#pragma once
#include <vector>

template<typename T>
class Component;

// Represented by IDs
// Populates scene
class Entity
{
public:
	Entity(int id);

	//void AddComponent(Component component);
	void GetComponent();
	void Update(float deltaTime);
	void Render();

	int GetId() const;

	bool operator==(const Entity& entity) const;

private:
	int id;
	//std::vector<Component> components;
};
