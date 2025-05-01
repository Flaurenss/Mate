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
#include "PhysicsComponent.h"

#include <thread>
#include <cassert>

JPH_SUPPRESS_WARNINGS

PhysicsEngine::PhysicsEngine()
{
	JPH::RegisterDefaultAllocator();
	JPH::Factory::sInstance = new JPH::Factory();
	JPH::RegisterTypes();

	tempAllocator = std::make_unique<JPH::TempAllocatorImpl>(32 * 1024 * 1024);
	jobSystem = std::make_unique<JPH::JobSystemThreadPool>(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);
	system = std::make_unique<JPH::PhysicsSystem>();
	bodyInterface = &system->GetBodyInterface();

	contactListener = std::make_unique<MateContactListener>(this);

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

	system->SetContactListener(contactListener.get());
}

PhysicsEngine::~PhysicsEngine()
{
	RmoveAndDestroyAll();

	JPH::UnregisterTypes();

	delete JPH::Factory::sInstance;
	JPH::Factory::sInstance = nullptr;
	
	tempAllocator = nullptr;
	jobSystem = nullptr;
	system = nullptr;

	broadPhaseLayerInterface = nullptr;
	objectVsBroadPhaseLayerFilter = nullptr;
	objectLayerPairFilter = nullptr;
}

void PhysicsEngine::Update(float fixedDeltaTime)
{
	// 1 assuming that delta time is 1/60 by default, if not we roundup based on actual fixedDeltaTime
	int cCollisionSteps = static_cast<int>(ceil(fixedDeltaTime / (1.0f / 60.0f)));
	system->Update(fixedDeltaTime, cCollisionSteps, tempAllocator.get(), jobSystem.get());
}

void PhysicsEngine::RegisterBody(
	int entityId,
	Vector3 halfExtents,
	Vector3 position,
	Vector3 eulerAngles,
	Entity entity)
{
	JPH::BoxShapeSettings shape_settings(JPH::Vec3(halfExtents.x, halfExtents.y, halfExtents.z));
	JPH::ShapeSettings::ShapeResult shape_result = shape_settings.Create();
	const JPH::Shape* shape = shape_result.Get();
	
	auto rotationQuat = EulerToQuat(eulerAngles);
	auto& physicsComponent = entity.GetComponent<PhysicsComponent>();
	auto eMotionType = MotionTypeToEMotionType(physicsComponent.BodyMotionType);
	auto layer = LayerToObjectLayer(physicsComponent.Layer);
	
	JPH::BodyCreationSettings settings(
		shape,
		JPH::Vec3(position.x, position.y, position.z),
		rotationQuat,
		eMotionType,
		layer);

	JPH::Body* body = bodyInterface->CreateBody(settings);
	body->SetUserData(entityId);
	body->SetIsSensor(physicsComponent.IsSensor());
	bodyInterface->AddBody(body->GetID(), JPH::EActivation::Activate);
	PhysicsData physicsData(body->GetID(), entity);
	entityPhysicsDataMap.insert(std::make_pair(entityId, physicsData));
}

Vector3 PhysicsEngine::GetPosition(int entityId)
{
	JPH::BodyID bodyId = GetBodyId(entityId);

	auto pos = bodyInterface->GetCenterOfMassPosition(bodyId);
	return Vector3(pos.GetX(), pos.GetY(), pos.GetZ());
}

Vector3 PhysicsEngine::GetEulerAngles(int entityId)
{
	JPH::BodyID bodyId = GetBodyId(entityId);

	auto rot = bodyInterface->GetRotation(bodyId);
	auto euler = rot.GetEulerAngles();
	return Vector3(
		MathUtils::degrees(euler.GetX()),
		MathUtils::degrees(euler.GetY()),
		MathUtils::degrees(euler.GetZ()));
}

void PhysicsEngine::SetPosition(int entityId, Vector3 position)
{
	JPH::BodyID bodyId = GetBodyId(entityId);
	auto jphPosition = JPH::RVec3(position.x, position.y, position.z);

	bodyInterface->SetPosition(bodyId, jphPosition, JPH::EActivation::Activate);
}

void PhysicsEngine::SetPositionAndRotation(int entityId, Vector3 position, Vector3 eulerAngles)
{
	JPH::BodyID bodyId = GetBodyId(entityId);
	auto rotationQuat = EulerToQuat(eulerAngles);

	auto jphPosition = JPH::RVec3(position.x, position.y, position.z);

	bodyInterface->SetPositionAndRotation(bodyId, jphPosition, rotationQuat, JPH::EActivation::Activate);
}

void PhysicsEngine::MoveKinematic(int entityId, Vector3 targetPosition, Vector3 targetRotation, float deltaTime)
{
	JPH::BodyID bodyId = GetBodyId(entityId);

	auto rotationQuat = EulerToQuat(targetRotation);

	bodyInterface->MoveKinematic(
		bodyId,
		JPH::RVec3(targetPosition.x, targetPosition.y, targetPosition.z),
		rotationQuat,
		deltaTime);
}

void PhysicsEngine::AddNewCollision(int entityA, int entityB)
{
	CollisionData newCollision {entityA, entityB};
	collisions.push_back(newCollision);
}

bool PhysicsEngine::IsRegistered(int entityId)
{
	auto it = entityPhysicsDataMap.find(entityId);
	if (it == entityPhysicsDataMap.end())
	{
		return false;
	}
	return true;
}

bool PhysicsEngine::TryGetBodyId(int entityId, JPH::BodyID& body)
{
	auto it = entityPhysicsDataMap.find(entityId);
	if (it == entityPhysicsDataMap.end())
	{
		return false;
	}
	body = it->second.BodyId;
	return true;
}

JPH::BodyID PhysicsEngine::GetBodyId(int entityId)
{
	JPH::BodyID bodyId;
	if (!TryGetBodyId(entityId, bodyId))
	{
		assert(false && "PhysicsEngine::GetBodyId: Entity ID not found in bodyMap");
	}
	return bodyId;
}

PhysicsData PhysicsEngine::GetEntityPhysicsData(int entityId)
{
	auto it = entityPhysicsDataMap.find(entityId);
	if (it == entityPhysicsDataMap.end())
	{
		assert(false && "PhysicsEngine::GetEntityPhysicsData: Entity ID not found in bodyMap");
	}
	return it->second;
}

std::vector<CollisionData> PhysicsEngine::GetCollisions()
{
	return collisions;
}

void PhysicsEngine::ClearCollisions()
{
	collisions.clear();
}

void PhysicsEngine::RmoveAndDestroy(int entityId)
{
	auto bodyId = GetBodyId(entityId);
	bodyInterface->RemoveBody(bodyId);
	bodyInterface->DestroyBody(bodyId);
}

void PhysicsEngine::RmoveAndDestroyAll()
{
	for (auto& physicalDataMap : entityPhysicsDataMap)
	{
		RmoveAndDestroy(physicalDataMap.first);
	}
}

JPH::Quat PhysicsEngine::EulerToQuat(Vector3 eulerAngles)
{
	return JPH::Quat::sRotation(JPH::Vec3::sAxisZ(), MathUtils::radians(eulerAngles.z)) *
		JPH::Quat::sRotation(JPH::Vec3::sAxisY(), MathUtils::radians(eulerAngles.y)) *
		JPH::Quat::sRotation(JPH::Vec3::sAxisX(), MathUtils::radians(eulerAngles.x));
}
