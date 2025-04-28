#pragma once
#include "MateMotionTypes.h"
#include "Vector.h"
#include <functional>
#include <string>


class Entity;

struct PhysicsComponent
{
public:
	MotionType BodyMotionType;
	std::function<void(Entity otherEntity)> OnCollide;

	PhysicsComponent();
	PhysicsComponent(MotionType type);

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
