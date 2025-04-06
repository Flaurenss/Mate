#include "ECS.h"
#include "Entity.h"

Entity ECS::CreateEntity()
{
	int entityId;

	if (freeIds.empty())
	{
		entityId = numEntities++;
		if (entityId >= entityComponentSignatures.size())
		{
			entityComponentSignatures.resize(entityId + 1);
		}
	}
	else
	{
		entityId = freeIds.front();
		freeIds.pop_front();
	}

	Entity entity(entityId, this);
	entitiesToAdd.insert(entity);
	return entity;
}

void ECS::DestroyEntity(Entity entity)
{
	entitiesToDestroy.insert(entity);
	const auto id = entity.GetId();
	freeIds.push_back(id);
}

void ECS::AddEntityToSystem(Entity entity)
{
	const auto entityId = entity.GetId();
	const auto& entityComponentSignature = entityComponentSignatures[entityId];

	for (auto& system : systems)
	{
		const auto& systemComponentSignature = system.second->GetComponentSignature();
		// Check if both signatures are same
		bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;
		if (isInterested)
		{
			system.second->AddEntity(entity);
		}
	}
}

void ECS::RemoveEntityFromSystems(Entity entity)
{
	for (auto& system : systems)
	{
		system.second->RemoveEntity(entity);
	}
}

void ECS::Update()
{
	for (auto entity : entitiesToAdd)
	{
		AddEntityToSystem(entity);
	}
	entitiesToAdd.clear();

	for (auto entity : entitiesToDestroy)
	{
		RemoveEntityFromSystems(entity);

		// Reset components for that entity
		entityComponentSignatures[entity.GetId()].reset();

		freeIds.push_back(entity.GetId());
	}
	entitiesToDestroy.clear();
}

void System::AddEntity(Entity entity)
{
	entities.push_back(entity);
}

void System::RemoveEntity(Entity entity)
{
	entities.erase(std::remove_if(entities.begin(), entities.end(), [&entity](Entity other)
		{
			return entity == other;
		}), entities.end());
}

std::vector<Entity> System::GetEntities() const
{
	return entities;
}

const Signature& System::GetComponentSignature() const
{
	return componentSignature;
}