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
	std::function<void(Entity otherPhysicsData)> OnCollide;

	PhysicsComponent();
	PhysicsComponent(MotionType type);
	PhysicsComponent(MotionType type, PhysicLayer layer);

	bool IsDirty() const;
	bool IsLayerDirty() const;
	bool IsResetForcesDirty() const;
	bool IsMoveToPositionDirty() const;
	bool IsInstantMoveToPositionDirty() const;

	void MoveKinematic(Vector3 newTargetPosition);
	void SetPosition(Vector3 newTargetPosition);
	void ResetForces();
	Vector3 GetActualKinematicTargetPosition() const;
	Vector3 GetActualInstantTargetPosition() const;
	
	void Reset();
	
	bool IsSensor() const;
	void SetIsSensor(bool status = true);

	void SetLayer(PhysicLayer newLayer);
	PhysicLayer GetLayer() const;

	std::string GetTag() const;
	void SetTag(std::string newTag);

private:
	std::string tag;
	PhysicLayer layer;
	bool isDirty = false;
	bool isLayerDirty = false;
	bool isResetForcesDirty = false;
	bool isMoveToPositionDirty = false;
	bool isInstantMoveToPositionDirty = false;
	bool isSensor = false;
	Vector3 kinematicTargetPosition = Vector3::Zero;
	Vector3 instantTargetPosition = Vector3::Zero;
};
