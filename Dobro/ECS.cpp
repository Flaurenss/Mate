#include "ECS.h"

int IComponent::nextId = 0;

Entity ECS::CreateEntity()
{
	int entityId = numEntities++;
	Entity entity(entityId);
	entitiesToAdd.insert(entity);
	return entity;
}

void ECS::AddEntityToSystem(Entity entity)
{

}

void ECS::Update()
{
	// TODO: add pending entities
	// TODO: remove ppending entities
}
