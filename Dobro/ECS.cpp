#include "ECS.h"
#include "ComponentRegistry.h"

Entity ECS::CreateEntity()
{
	int entityId = numEntities++;
	Entity entity(entityId);
	entitiesToAdd.insert(entity);
	return entity;
}

template<typename TComponent, typename ...TArgs>
void ECS::AddComponent(Entity entity, TArgs && ...args)
{
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	// Check and resize if needed (when TComponent has not been add)
	if (componentId >= componentsRegistry.size())
	{
		componentsRegistry.resize(componentId +1, nullptr);
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

	return componentsRegistry[componentId].Get(entityId);
}

void ECS::Update()
{
	// TODO: add pending entities
	// TODO: remove ppending entities
}
