#include "PhysicsSystem.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "EnableComponent.h"
#include "Logger.h"
#include "MeshComponent.h"

PhysicsSystem::PhysicsSystem()
{
	phyEngine = std::make_unique<PhysicsEngine>();
	RequireComponent<TransformComponent>();
	RequireComponent<PhysicsComponent>();
}

void PhysicsSystem::Update(float fixedDeltaTime)
{
	for (Entity& entity : GetEntities())
	{
		if (entity.HasComponent<EnableComponent>()
			&& !entity.GetComponent<EnableComponent>().Enabled)
		{
			continue;
		}
		auto& physicsComponent = entity.GetComponent<PhysicsComponent>();
		auto transform = entity.GetComponent<TransformComponent>();
		if (phyEngine->IsRegistered(entity.GetId()))
		{
			// Register new body
			Vector3 position = transform.Position;
			Vector3 extents = Vector3::One;
			if (entity.HasComponent<MeshComponent>())
			{
				extents = entity.GetComponent<MeshComponent>().GetExtents();
			}
			auto halfExtents = (extents * transform.Scale) /2;
			halfExtents.x = std::max(halfExtents.x, 0.05f);
			halfExtents.y = std::max(halfExtents.y, 0.05f);
			halfExtents.z = std::max(halfExtents.z, 0.05f);
			phyEngine->RegisterBody(
				entity.GetId(),
				halfExtents,
				position,
				transform.EulerAngles,
				physicsComponent.motionType,
				physicsComponent.IsSensor(),
				physicsComponent.OnCollide);
		}
		else
		{
			if (physicsComponent.IsDirty())
			{
				phyEngine->MoveKinematic(
					entity.GetId(),
					physicsComponent.GetActualTargetPosition(),
					transform.EulerAngles,
					fixedDeltaTime);
				physicsComponent.Reset();
			}
		}
	}

	phyEngine->Update(fixedDeltaTime);

	for (auto collisionData : phyEngine->collisions)
	{
		CallOnCollisionData(collisionData.EntityIdA, collisionData.EntityIdB);
		CallOnCollisionData(collisionData.EntityIdB, collisionData.EntityIdA);
	}

	phyEngine->collisions.clear();

	for (Entity& entity : GetEntities())
	{
		if (entity.HasComponent<EnableComponent>()
			&& !entity.GetComponent<EnableComponent>().Enabled)
		{
			continue;
		}

		auto phyComponent = entity.GetComponent<PhysicsComponent>();
		if (phyComponent.motionType != STATIC)
		{
			auto& transformComponent = entity.GetComponent<TransformComponent>();
			Vector3 newPosition = phyEngine->GetPosition(entity.GetId());
			Vector3 newEulerRotation = phyEngine->GetEulerAngles(entity.GetId());
			transformComponent.Position = newPosition;
			transformComponent.EulerAngles = newEulerRotation;
		}
	}
}

void PhysicsSystem::CallOnCollisionData(int selfId, int otherId)
{
	auto physicsDataA = phyEngine->GetEntityPhysicsData(selfId);
	auto physicsDataB = phyEngine->GetEntityPhysicsData(otherId);
	auto& entityCallBack = physicsDataA.OnCollide;
	if (entityCallBack)
	{
		entityCallBack(physicsDataB.Entity);
	}
}
