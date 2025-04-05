#pragma once
#include <vector>
#include "System.h"
#include "Entity.h"
#include "ComponentRegistry.h"
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <set>

class IRegistry;

/// <summary>
/// ECS administrator.
/// Manages creation and destruction of entities, systems and components.
/// </summary>
class ECS
{
public:
	ECS() = default;
	Entity CreateEntity();
	void DestroyEntity();

	template <typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
	template <typename TComponent> void RemoveComponent(Entity entity);
	template <typename TComponent> bool HasComponent(Entity entity) const;
	template <typename TComponent> TComponent& GetComponent(Entity entity) const;

	/// <summary>
	/// Add entity to system if entity components match sytem components requirements.
	/// </summary>
	/// <param name="entity"></param>
	void AddEntityToSystem(Entity entity);
	template <typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);
	template <typename TSystem> void RemoveSystem();
	template <typename TSystem> bool HasSystem() const;
	template <typename TSystem> TSystem& GetSystem() const;
	
	/// <summary>
	/// Add or Destroy entities based on pending set to add and pending set to destroy.
	/// This should be executed at the end of the frame.
	/// </summary>
	void Update();

private:
	int numEntities;

	std::set<Entity> entitiesToAdd;
	std::set<Entity> entitiesToDestroy;

	// ComponentRegistry concept:
	//  [0].........[1] [2] [3]
	//componentID
	//   |
	//   v
	//  [0] ->EntityId n0 
	//  [1] -> EntityId n1
	// Each component represents all data for a type Component.
	// Vector index = component id
	// Component Registry index = entityid
	std::vector<IRegistry*> componentsRegistry;

	// The signature lets us know which component are active for an entity.
	// Vector index = entity id
	std::vector<Signature> entityComponentSignatures;

	// Map of active systems where index = system type
	std::unordered_map<std::type_index, System*> systems;
};

template<typename TComponent, typename ...TArgs>
void ECS::AddComponent(Entity entity, TArgs && ...args)
{
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	// Check and resize if needed (when TComponent has not been add)
	if (componentId >= componentsRegistry.size())
	{
		componentsRegistry.resize(componentId + 1, nullptr);
	}

	// Check if TComponent Registry exists, ifnot it created a new one
	if (!componentsRegistry[componentId])
	{
		ComponentRegistry<TComponent>* newRegistry = new ComponentRegistry<TComponent>();
		componentsRegistry[componentId] = newRegistry;
	}

	ComponentRegistry<TComponent>* componentRegistry = ComponentRegistry<TComponent>(componentsRegistry[componentId]);

	if (entityId > componentRegistry->GetSize())
	{
		componentRegistry->Resize(numEntities);
	}

	// Send multiple arguments to the specified new TComponent
	TComponent newComponent(std::forward<TArgs>(args)...);
	componentRegistry->Set(entityId, newComponent);
	entityComponentSignatures[entityId].set(componentId);
}

template<typename TComponent>
void ECS::RemoveComponent(Entity entity)
{
	const auto entityId = entity.GetId();
	const auto componentId = Component<TComponent>::GetId();

	// Put TComponent bit from entity at false:
	entityComponentSignatures[entityId].set(componentId, false);
}

template<typename TComponent>
bool ECS::HasComponent(Entity entity) const
{
	const auto entityId = entity.GetId();
	const auto componentId = Component<TComponent>::GetId();

	return entityComponentSignatures[entityId].test(componentId);
}

template<typename TComponent>
TComponent& ECS::GetComponent(Entity entity) const
{
	const auto entityId = entity.GetId();
	const auto componentId = Component<TComponent>::GetId();

	auto componentRegistry = std::static_pointer_cast<ComponentRegistry<TComponent>>(componentsRegistry[componentId]);
	return componentRegistry->Get(entityId);
}

template<typename TSystem, typename ...TArgs>
void ECS::AddSystem(TArgs && ...args)
{
	auto typeIndex = std::type_index(typeid(TSystem));
	TSystem* newSystem(new TSystem(std::forward<TArgs>(args)...));
	systems.insert(std::make_pair(typeIndex, newSystem));
}

template<typename TSystem>
void ECS::RemoveSystem()
{
	auto typeIndex = std::type_index(typeid(TSystem));
	auto system = systems.find(typeIndex);
	systems.erase(system);
}

template<typename TSystem>
bool ECS::HasSystem() const
{
	auto typeIndex = std::type_index(typeid(TSystem));
	return systems.find(typeIndex != systems.end());
}

template<typename TSystem>
TSystem& ECS::GetSystem() const
{
	auto typeIndex = std::type_index(typeid(TSystem));
	auto system = systems.find(typeIndex);
	return *(std::static_pointer_cast<TSystem>(system->second));
}