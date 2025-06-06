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
		if (!phyEngine->IsRegistered(entity.GetId()))
		{
			RegisterBody(entity);
		}

		if (entity.HasComponent<EnableComponent>()
			&& !entity.GetComponent<EnableComponent>().Enabled)
		{
			phyEngine->DeactivateBody(entity.GetId());
			continue;
		}

		phyEngine->ActivateBody(entity.GetId());

		auto& physicsComponent = entity.GetComponent<PhysicsComponent>();
		auto& transform = entity.GetComponent<TransformComponent>();

		if (physicsComponent.IsDirty())
		{
			if (physicsComponent.BodyMotionType == KINEMATIC)
			{
				phyEngine->MoveKinematic(
					entity.GetId(),
					physicsComponent.GetActualKinematicTargetPosition(),
					transform.EulerAngles,
					fixedDeltaTime);
			}
			else if (physicsComponent.BodyMotionType == STATIC)
			{
				phyEngine->SetPositionAndRotation(
					entity.GetId(),
					transform.Position,
					transform.EulerAngles);
			}
		}
		if (physicsComponent.IsResetForcesDirty())
		{
			phyEngine->ResetBodyForces(entity.GetId());
		}
		if (physicsComponent.IsInstantMoveToPositionDirty())
		{
			phyEngine->ResetBodyForces(entity.GetId());
			phyEngine->SetPositionAndRotation(
				entity.GetId(),
				transform.Position,
				transform.EulerAngles);
		}

		if (physicsComponent.IsLayerDirty())
		{
			phyEngine->SetLayer(entity.GetId(), physicsComponent.GetLayer());
		}

		physicsComponent.Reset();
	}

	phyEngine->Update(fixedDeltaTime);

	for (auto& collisionData : phyEngine->GetCollisions())
	{
		CallOnCollisionData(collisionData.EntityIdA, collisionData.EntityIdB);
		CallOnCollisionData(collisionData.EntityIdB, collisionData.EntityIdA);
	}

	phyEngine->ClearCollisions();

	for (Entity& entity : GetEntities())
	{
		if (entity.HasComponent<EnableComponent>()
			&& !entity.GetComponent<EnableComponent>().Enabled)
		{
			continue;
		}

		auto& phyComponent = entity.GetComponent<PhysicsComponent>();
		if (phyComponent.BodyMotionType != STATIC)
		{
			auto& transformComponent = entity.GetComponent<TransformComponent>();
			Vector3 newPosition = phyEngine->GetPosition(entity.GetId());
			Vector3 newEulerRotation = phyEngine->GetEulerAngles(entity.GetId());
			transformComponent.Position = newPosition;
			//transformComponent.EulerAngles = newEulerRotation;
		}
	}
}

void PhysicsSystem::RegisterBody(Entity& entity)
{
	// Register new body
	auto& physicsComponent = entity.GetComponent<PhysicsComponent>();
	auto& transform = entity.GetComponent<TransformComponent>();
	Vector3 extents = Vector3::One;
	if (entity.HasComponent<MeshComponent>())
	{
		extents = entity.GetComponent<MeshComponent>().GetExtents();
	}
	auto halfExtents = (extents / 2 ) * transform.Scale;
	halfExtents.x = std::max(halfExtents.x, 0.05f);
	halfExtents.y = std::max(halfExtents.y, 0.05f);
	halfExtents.z = std::max(halfExtents.z, 0.05f);
	
	phyEngine->RegisterBody(
		entity.GetId(),
		halfExtents,
		transform.Position,
		transform.EulerAngles,
		entity);
}

void PhysicsSystem::CallOnCollisionData(int selfId, int otherId)
{
	auto physicsDataA = phyEngine->GetEntityPhysicsData(selfId);
	auto physicsDataB = phyEngine->GetEntityPhysicsData(otherId);
	auto& entityCallBack = physicsDataA.GetPhysicsComponent().OnCollide;

	if (entityCallBack)
	{
		entityCallBack(physicsDataB.GetEntity());
	}
}
