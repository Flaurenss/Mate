#pragma once
#include <Jolt/Jolt.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSystem.h>

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
private:
	const unsigned int cMaxBodies = 65536; // Recommended value by Jolt
	const unsigned int cNumBodyMutexes = 0;
	const unsigned int cMaxBodyPairs = 1024;
	const unsigned int cMaxContactConstraints = 65536; // Recommended value by Jolt

	std::unique_ptr<JPH::TempAllocatorImpl> tempAllocator;
	std::unique_ptr<JPH::JobSystemThreadPool> jobSystem;

	std::unique_ptr<JPH::PhysicsSystem> system;

	std::unique_ptr<JPH::BroadPhaseLayerInterface> broadPhaseLayerInterface;
	std::unique_ptr<JPH::ObjectVsBroadPhaseLayerFilter> objectVsBroadPhaseLayerFilter;
	std::unique_ptr<JPH::ObjectLayerPairFilter> objectLayerPairFilter;
};

