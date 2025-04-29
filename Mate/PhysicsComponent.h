#pragma once
#include "MateMotionTypes.h"
#include <functional>
#include <string>
#include "Vector.h"
#include "PhysicLayer.h"


class Entity;

struct PhysicsComponent
{
public:
	MotionType BodyMotionType;
	PhysicLayer Layer;
	std::function<void(Entity otherPhysicsData)> OnCollide;

	PhysicsComponent();
	PhysicsComponent(MotionType type);
	PhysicsComponent(MotionType type, PhysicLayer layer);

	bool IsDirty() const;
	void SetDirty(bool status = true);
	
	void MoveKinematic(Vector3 newTargetPosition);
	Vector3 GetActualTargetPosition() const;
	
	void Reset();
	
	bool IsSensor() const;
	void SetIsSensor(bool status = true);

	std::string GetTag() const;
	void SetTag(std::string newTag);

private:
	std::string tag;
	bool isDirty = false;
	bool isSensor = false;
	Vector3 targetPosition = Vector3::Zero;
};
