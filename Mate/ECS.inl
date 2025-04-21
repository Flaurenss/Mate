#pragma once

#include "ECS.h"
#include "Entity.h"

template<typename TComponent, typename ...TArgs>
TComponent& ECS::AddComponent(Entity entity, TArgs && ...args)
{
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	// Check and resize if needed (when TComponent has not been add)
	if (componentId >= componentsRegistry.size())
	{
		componentsRegistry.resize(componentId + 1, nullptr);
	}

	// Check if TComponent Registry exists, if not it creates a new one
	if (!componentsRegistry[componentId])
	{
		std::shared_ptr<ComponentRegistry<TComponent>> newRegistry(new ComponentRegistry<TComponent>());
		componentsRegistry[componentId] = newRegistry;
	}

	std::shared_ptr<ComponentRegistry<TComponent>> componentRegistry =
		std::static_pointer_cast<ComponentRegistry<TComponent>>(componentsRegistry[componentId]);

	/* Send multiple arguments to the specified new TComponent*/
	TComponent newComponent(std::forward<TArgs>(args)...);
	componentRegistry->Set(entityId, newComponent);
	entityComponentSignatures[entityId].set(componentId);

	return componentRegistry->Get(entityId);
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