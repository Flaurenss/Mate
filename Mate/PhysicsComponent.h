#pragma once
#include "MateMotionTypes.h"
#include "Vector.h"

struct PhysicsComponent
{
public:
	MotionType motionType;

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
private:
	bool isDirty = false;
	Vector3 targetPosition = Vector3::Zero;
};
