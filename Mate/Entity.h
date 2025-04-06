#pragma once
#include <vector>
#include <memory>
#include "ComponentManager.h"

// Represented by IDs
// Populates scene
class Entity
{
public:
	Entity(int id);
	~Entity();

	template <typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
	template <typename TComponent> void RemoveComponent();
	template <typename TComponent> bool HasComponent() const;
	template <typename TComponent> TComponent& GetComponent() const;

	int GetId() const;

	bool operator==(const Entity& entity) const;
	bool operator<(const Entity& entity) const;
	bool operator>(const Entity& entity) const;

private:
	int id;
	std::shared_ptr<ComponentManager> componentManager;
};

template<typename TComponent, typename ...TArgs>
inline void Entity::AddComponent(TArgs && ...args)
{
}

template<typename TComponent>
inline void Entity::RemoveComponent()
{
}

template<typename TComponent>
inline bool Entity::HasComponent() const
{
	return componentManager->HasComponent<TComponent>(this);
}

template<typename TComponent>
inline TComponent& Entity::GetComponent() const
{
	// TODO: insert return statement here
}
