#include "Entity.h"

Entity::Entity(int id) : id(id)
{
}

Entity::~Entity()
{
}

int Entity::GetId() const
{
	return id;
}

bool Entity::operator==(const Entity& entity) const
{
	return id == entity.GetId();
}

bool Entity::operator<(const Entity& entity) const
{
	return id < entity.GetId();
}

bool Entity::operator>(const Entity& entity) const
{
	return id > entity.GetId();
}