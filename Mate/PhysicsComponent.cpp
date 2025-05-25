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

bool PhysicsComponent::IsResetForcesDirty() const
{
	return isResetForcesDirty;
}

bool PhysicsComponent::IsMoveToPositionDirty() const
{
	return isMoveToPositionDirty;
}

bool PhysicsComponent::IsInstantMoveToPositionDirty() const
{
	return isInstantMoveToPositionDirty;
}

void PhysicsComponent::MoveKinematic(Vector3 newTargetPosition)
{
	kinematicTargetPosition = newTargetPosition;
	isDirty = true;
}

void PhysicsComponent::SetPosition(Vector3 newTargetPosition)
{
	isInstantMoveToPositionDirty = true;
}

void PhysicsComponent::ResetForces()
{
	isResetForcesDirty = true;
}

Vector3 PhysicsComponent::GetActualKinematicTargetPosition() const
{
	return kinematicTargetPosition;
}

Vector3 PhysicsComponent::GetActualInstantTargetPosition() const
{
	return instantTargetPosition;
}

void PhysicsComponent::Reset()
{
	isDirty = false;
	isLayerDirty = false;
	isResetForcesDirty = false;
	kinematicTargetPosition = Vector3::Zero;
	instantTargetPosition = Vector3::Zero;
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