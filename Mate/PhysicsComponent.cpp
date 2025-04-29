#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent() : BodyMotionType(DYNAMIC), Layer(PhysicLayer::MOVING)
{}

PhysicsComponent::PhysicsComponent(MotionType type) : BodyMotionType(type), Layer(PhysicLayer::MOVING)
{}

PhysicsComponent::PhysicsComponent(MotionType type, PhysicLayer layer) : BodyMotionType(type), Layer(layer)
{}

bool PhysicsComponent::IsDirty() const
{
	return isDirty;
}

void PhysicsComponent::SetDirty(bool status)
{
	isDirty = status;
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

std::string PhysicsComponent::GetTag() const
{
	return tag;
}

void PhysicsComponent::SetTag(std::string newTag)
{
	tag = newTag;
}