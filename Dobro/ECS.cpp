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

	// Check if TComponent Registry exists
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

	// Send multiple arguments to the specified new TComponent.
	TComponent newComponent(std::forward<TArgs>(args)...);
	componentRegistry->Set(entityId, newComponent);
	entityComponentSignatures[entityId].set(componentId);
}

void ECS::Update()
{
	// TODO: add pending entities
	// TODO: remove ppending entities
}
