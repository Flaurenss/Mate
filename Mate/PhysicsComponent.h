#pragma once
#include "MateMotionTypes.h"
#include "Vector.h"
#include <functional>
#include <string>


class Entity;

struct PhysicsComponent
{
public:
	MotionType motionType;

	std::function<void(Entity otherEntity)> OnCollide;

	PhysicsComponent() : motionType(STATIC) {}
	PhysicsComponent(MotionType type) : motionType(type) {}

	bool IsDirty() { return isDirty; }
	void MoveKinematic(Vector3 newTargetPosition)
	{
		targetPosition = newTargetPosition;
		isDirty = true;
	}
	Vector3 GetActualTargetPosition()
	{
		return targetPosition;
	}
	void SetDirty(bool status = true)
	{
		isDirty = status;
	}
	void Reset()
	{
		isDirty = false;
		targetPosition = Vector3::Zero;
	}
	bool IsSensor() { return isSensor; }
	void SetIsSensor(bool status = true)
	{
		isSensor = status;
	}

	void SetTag(std::string newTag)
	{
		tag = newTag;
	}
	std::string GetTag() { return tag; }

private:
	std::string tag;
	bool isDirty = false;
	bool isSensor = false;
	Vector3 targetPosition = Vector3::Zero;
};
