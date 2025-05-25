#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent() : BodyMotionType(DYNAMIC), layer(PhysicLayer::MOVING)
{}

PhysicsComponent::PhysicsComponent(MotionType type) : BodyMotionType(type), layer(PhysicLayer::MOVING)
{}

PhysicsComponent::PhysicsComponent(MotionType type, PhysicLayer layer) : BodyMotionType(type), layer(layer)
{}

bool PhysicsComponent::IsDirty() const
{
	return isDirty;
}

bool PhysicsComponent::IsLayerDirty() const
{
	return isLayerDirty;
}

void PhysicsComponent::MoveKinematic(Vector3 newTargetPosition)
{
	targetPosition = newTargetPosition;
	isDirty = true;
}

Vector3 PhysicsComponent::GetActualTargetPosition() const
{
	return targetPosition;
}

void PhysicsComponent::Reset()
{
	isDirty = false;
	isLayerDirty = false;
	targetPosition = Vector3::Zero;
}

bool PhysicsComponent::IsSensor() const
{
	return isSensor;
}

void PhysicsComponent::SetIsSensor(bool status)
{
	isSensor = status;
}

void PhysicsComponent::SetLayer(PhysicLayer newLayer)
{
	layer = newLayer;
	isLayerDirty = true;
}

PhysicLayer PhysicsComponent::GetLayer() const
{
	return layer;
}

std::string PhysicsComponent::GetTag() const
{
	return tag;
}

void PhysicsComponent::SetTag(std::string newTag)
{
	tag = newTag;
}