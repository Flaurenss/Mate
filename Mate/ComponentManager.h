#pragma once

class Entity;
class ECS;

class ComponentManager
{
public:
	ComponentManager(ECS* ecs)
	{
		orch = ecs;
	}
	template <typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
	template <typename TComponent> void RemoveComponent(Entity entity);
	template <typename TComponent> bool HasComponent(Entity entity) const;
	template <typename TComponent> TComponent& GetComponent(Entity entity) const;
private:
	ECS* orch;
};

template<typename TComponent, typename ...TArgs>
inline void ComponentManager::AddComponent(Entity entity, TArgs && ...args)
{
}

template<typename TComponent>
inline void ComponentManager::RemoveComponent(Entity entity)
{
}

template<typename TComponent>
inline bool ComponentManager::HasComponent(Entity entity) const
{
	return false;
}

template<typename TComponent>
inline TComponent& ComponentManager::GetComponent(Entity entity) const
{
	// TODO: insert return statement here
}
