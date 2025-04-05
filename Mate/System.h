#pragma once
#include <bitset>
#include <vector>
#include "Entity.h"
#include "Component.h"

const unsigned int MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> Signature;

// Transforms Components from state A to state B
// Performs logic on components and entities
class System
{
public:
	System() = default;
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

template<typename TComponent>
void System::RequireComponent()
{
	const auto componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
}
