#include "Entity.h"
#include <memory>

Entity::Entity(int id) : id(id)
{ }

Entity::~Entity()
{
	//delete ecs;
	//ecs = nullptr;
}

//void Entity::SetECS(ECS* ecs)
//{
//	this->ecs = ecs;
//}

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
