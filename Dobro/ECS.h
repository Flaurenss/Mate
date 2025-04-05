#pragma once
#include <vector>
#include "System.h"
#include "Entity.h"
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
	template <typename TComponent> void HasComponent(Entity entity) const;
	template <typename TComponent> TComponent& GetComponent(Entity entity) const;

	void AddEntityToSystem(Entity entity);

	void AddSystem();
	System GetSystem();
	/*void RemoveSystem();
	bool HasSystem();*/
	
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
