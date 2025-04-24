#include "PhysicsEngine.h"
#include "CommonPhysics.h"
#include <Jolt/RegisterTypes.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

#include "MathUtils.h"
#include "BPLayerInterfaceImpl.h"
#include "ObjectVsBPLayerFilterImpl.h"
#include "ObjectLayerPairFilterImpl.h"

#include <thread>
#include <cassert>

PhysicsEngine::PhysicsEngine()
{
	JPH::RegisterDefaultAllocator();
	JPH::Factory::sInstance = new JPH::Factory();
	JPH::RegisterTypes();

	//tempAllocator = std::make_unique<JPH::TempAllocatorMalloc>();
	tempAllocator = std::make_unique<JPH::TempAllocatorImpl>(32 * 1024 * 1024);
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
	system->Update(fixedDeltaTime, cCollisionSteps, tempAllocator.get(), jobSystem.get());
}

void PhysicsEngine::RegisterBody(int entityId, Vector3 halfExtents, Vector3 position, Vector3 eulerAngles, MotionType motionType)
{
	JPH::BoxShapeSettings shape_settings(JPH::Vec3(halfExtents.x, halfExtents.y, halfExtents.z));
	JPH::ShapeSettings::ShapeResult shape_result = shape_settings.Create();
	const JPH::Shape* shape = shape_result.Get();
	
	JPH::Quat rotationQuat = JPH::Quat::sRotation(JPH::Vec3::sAxisZ(), MathUtils::radians(eulerAngles.z))
		* JPH::Quat::sRotation(JPH::Vec3::sAxisY(), MathUtils::radians(eulerAngles.y))
		* JPH::Quat::sRotation(JPH::Vec3::sAxisX(), MathUtils::radians(eulerAngles.x));

	auto eMotionType = MotionTypeToEMotionType(motionType);
	JPH::ObjectLayer layer = Layers::NON_MOVING;
	if (motionType == STATIC)
	{
		layer = Layers::NON_MOVING;
	}
	JPH::BodyCreationSettings settings(
		shape,
		JPH::Vec3(position.x, position.y, position.z),
		rotationQuat,
		eMotionType,
		Layers::MOVING);

	JPH::Body* body = system->GetBodyInterface().CreateBody(settings);
	system->GetBodyInterface().AddBody(body->GetID(), JPH::EActivation::Activate);
	bodyMap.insert(std::make_pair(entityId, body->GetID()));
}

Vector3 PhysicsEngine::GetPosition(int entityId)
{
	auto it = bodyMap.find(entityId);
	if (it != bodyMap.end())
	{
		const JPH::BodyID& bodyID = it->second;
		auto& interface = system->GetBodyInterface();
		auto pos = interface.GetCenterOfMassPosition(bodyID);
		return Vector3(pos.GetX(), pos.GetY(), pos.GetZ());
	}

	assert(false && "PhysicsEngine::GetPosition: Entity ID not found in bodyMap");
}
