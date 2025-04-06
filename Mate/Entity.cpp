#include "Entity.h"

Entity::Entity(int id, ECS* registry) : id(id), registry(registry)
{ }

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