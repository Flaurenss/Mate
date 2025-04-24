#include "PhysicsEngine.h"
#include <Jolt/RegisterTypes.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

#include "BPLayerInterfaceImpl.h"
#include "ObjectVsBPLayerFilterImpl.h"
#include "ObjectLayerPairFilterImpl.h"

#include <thread>

PhysicsEngine::PhysicsEngine()
{
	JPH::RegisterDefaultAllocator();
	JPH::Factory::sInstance = new JPH::Factory();
	JPH::RegisterTypes();

	tempAllocator = std::make_unique<JPH::TempAllocatorImpl>(10 * 1024 * 1024);
	jobSystem = std::make_unique<JPH::JobSystemThreadPool>(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);
	system = std::make_unique<JPH::PhysicsSystem>();

	broadPhaseLayerInterface = std::make_unique<BPLayerInterfaceImpl>();
	objectVsBroadPhaseLayerFilter = std::make_unique<ObjectVsBPLayerFilterImpl>();
	objectLayerPairFilter = std::make_unique<ObjectLayerPairFilterImpl>();
	
	system->Init(
		cMaxBodies,
		cNumBodyMutexes,
		cMaxBodyPairs,
		cMaxContactConstraints,
		*broadPhaseLayerInterface,
		*objectVsBroadPhaseLayerFilter,
		*objectLayerPairFilter
	);
}

PhysicsEngine::~PhysicsEngine()
{
	JPH::UnregisterTypes();
	delete JPH::Factory::sInstance;
	JPH::Factory::sInstance = nullptr;
	tempAllocator = nullptr;
	jobSystem = nullptr;
}

void PhysicsEngine::Update(float fixedDeltaTime)
{
	// 1 assuming that delta time is 1/60 by default, if not we roundup based on actual fixedDeltaTime
	int cCollisionSteps = static_cast<int>(ceil(fixedDeltaTime / (1.0f / 60.0f)));
	system->Update(fixedDeltaTime, 1, tempAllocator.get(), jobSystem.get());
}
