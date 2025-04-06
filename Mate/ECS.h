#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <set>
#include <bitset>
#include "ComponentRegistry.h"
#include "Component.h"
#include <deque>

class Entity;
class IRegistry;

const unsigned int MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> Signature;

/// <summary>
/// Transforms Components from state A to state B and performs logic on components and entities.
/// </summary>
class System
{
public:
	//System() = default;
	~System() = default;

	void AddEntity(Entity entity);
	void RemoveEntity(Entity entity);
	std::vector<Entity> GetEntities() const;
	const Signature& GetComponentSignature() const;

	virtual void Update() = 0;

	/// <summary>
	/// Define the component type TComponent that entities must have to be consdiered by the system.
	/// </summary>
	/// <typeparam name="TComponent">The Component type.</typeparam>
	template <typename TComponent> void RequireComponent();
private:
	std::vector<Entity> entities;
	Signature componentSignature;
};

/// <summary>
/// ECS administrator.
/// Manages creation and destruction of entities, components and systems.
/// </summary>
class ECS
{
public:
	ECS() = default;
	Entity CreateEntity();
	void DestroyEntity(Entity entity);

	template <typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
	template <typename TComponent> void RemoveComponent(Entity entity);
	template <typename TComponent> bool HasComponent(Entity entity) const;
	template <typename TComponent> TComponent& GetComponent(Entity entity) const;

	/// <summary>
	/// Add entity to system if entity components match sytem components requirements.
	/// </summary>
	/// <param name="entity">The entity to add.</param>
	void AddEntityToSystem(Entity entity);
	void RemoveEntityFromSystems(Entity entity);
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
	int numEntities = 0;

	std::set<Entity> entitiesToAdd;
	std::set<Entity> entitiesToDestroy;
	std::deque<int> freeIds;

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
	std::vector<std::shared_ptr<IRegistry>> componentsRegistry;

	// The signature lets us know which component are active for an entity.
	// Vector index = entity id
	std::vector<Signature> entityComponentSignatures;

	// Map of active systems where index = system type
	std::unordered_map<std::type_index, std::shared_ptr<System>> systems;
};
#include "ECS.inl"
template<typename TSystem, typename ...TArgs>
void ECS::AddSystem(TArgs && ...args)
{
	auto typeIndex = std::type_index(typeid(TSystem));

	std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
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

template<typename TComponent>
void System::RequireComponent()
{
	const auto componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
}
