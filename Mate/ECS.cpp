#include "ECS.h"
#include "Entity.h"

Entity ECS::CreateEntity()
{
	int entityId = numEntities++;
	Entity entity(entityId, this);
	entitiesToAdd.insert(entity);
	if (entityId >= entityComponentSignatures.size())
	{
		entityComponentSignatures.resize(entityId + 1);
	}
	return entity;
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

void ECS::Update()
{
	for (auto entity : entitiesToAdd)
	{
		AddEntityToSystem(entity);
	}
	entitiesToAdd.clear();

	//for (auto entity : entitiesToDestroy)
	//{
	//	DestroyEntity(entity);
	//}
	//entitiesToDestroy.clear();
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