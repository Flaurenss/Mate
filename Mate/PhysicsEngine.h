#pragma once
#include <Jolt/Jolt.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSystem.h>

#include <unordered_map>
#include "Vector.h"
#include "MateMotionTypes.h"
#include "MateContactListener.h"
#include "PhysicsData.h"
#include "CollisionData.h"

/// <summary>
/// Physics engine consuming Jolt library: https://github.com/jrouwe/JoltPhysics
/// Alpha implementation following best practices described on their samples.
/// </summary>
class PhysicsEngine
{
public:
	PhysicsEngine();
	~PhysicsEngine();

	void Update(float deltaTime);
	void RegisterBody(
		int entityId,
		Vector3 halfExtents,
		Vector3 position,
		Vector3 eulerAngles,
		Entity entity);
	
	void ActivateBody(int entityId);
	void DeactivateBody(int entityId);

	Vector3 GetPosition(int entityId);
	Vector3 GetEulerAngles(int entityId);

	void SetLayer(int entityId, PhysicLayer layer);

	void ResetBodyForces(int entityId);

	void SetPosition(int entityId, Vector3 position);
	void SetPositionAndRotation(int entityId, Vector3 position, Vector3 eulerAngles);
	void MoveKinematic(int entityId, Vector3 targetPosition, Vector3 targetRotation, float deltaTime);

	void AddNewCollision(int entityA, int entityB);

	bool IsRegistered(int entityId);
	bool TryGetBodyId(int entityId, JPH::BodyID& body);
	JPH::BodyID GetBodyId(int entityId);
	PhysicsData GetEntityPhysicsData(int entityId);

	std::vector<CollisionData> GetCollisions();
	void ClearCollisions();

	void RmoveAndDestroy(int entityId);
	void RmoveAndDestroyAll();

private:
	std::vector<CollisionData> collisions;
	std::unordered_map<int, PhysicsData> entityPhysicsDataMap;

	const unsigned int cMaxBodies = 65536; // Recommended value by Jolt
	const unsigned int cNumBodyMutexes = 0;
	const unsigned int cMaxBodyPairs = 65536;
	const unsigned int cMaxContactConstraints = 10240; // Recommended value by Jolt

	std::unique_ptr<JPH::TempAllocator> tempAllocator;
	std::unique_ptr<JPH::JobSystemThreadPool> jobSystem;

	std::unique_ptr<JPH::PhysicsSystem> system;
	std::unique_ptr<MateContactListener> contactListener;
	JPH::BodyInterface* bodyInterface;

	std::unique_ptr<JPH::BroadPhaseLayerInterface> broadPhaseLayerInterface;
	std::unique_ptr<JPH::ObjectVsBroadPhaseLayerFilter> objectVsBroadPhaseLayerFilter;
	std::unique_ptr<JPH::ObjectLayerPairFilter> objectLayerPairFilter;

	JPH::Quat EulerToQuat(Vector3 eulerAngles);
};