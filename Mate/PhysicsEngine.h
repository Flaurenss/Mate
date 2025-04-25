#pragma once
#include <Jolt/Jolt.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSystem.h>

#include <unordered_map>
#include "Vector.h"
#include "MateMotionTypes.h"

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
	void RegisterBody(int entityId, Vector3 halfExtents, Vector3 position, Vector3 eulerAngles, MotionType mode);
	Vector3 GetPosition(int entityId);
	Vector3 GetEulerAngles(int entityId);
	Vector3 SetPosition(int entityId, Vector3 position);

	bool TryGetBodyId(int entityId, JPH::BodyID& body);

	// Map Entity ID to Jolt Body ID
	std::unordered_map<int, JPH::BodyID> bodyMap;
private:
	const unsigned int cMaxBodies = 65536; // Recommended value by Jolt
	const unsigned int cNumBodyMutexes = 0;
	const unsigned int cMaxBodyPairs = 65536;
	const unsigned int cMaxContactConstraints = 10240; // Recommended value by Jolt

	std::unique_ptr<JPH::TempAllocator> tempAllocator;
	std::unique_ptr<JPH::JobSystemThreadPool> jobSystem;

	std::unique_ptr<JPH::PhysicsSystem> system;

	std::unique_ptr<JPH::BroadPhaseLayerInterface> broadPhaseLayerInterface;
	std::unique_ptr<JPH::ObjectVsBroadPhaseLayerFilter> objectVsBroadPhaseLayerFilter;
	std::unique_ptr<JPH::ObjectLayerPairFilter> objectLayerPairFilter;

};

