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
		if (phyEngine->bodyMap.find(entity.GetId()) == phyEngine->bodyMap.end())
		{
			// TODO: register new body
			auto transform = entity.GetComponent<TransformComponent>();
			Vector3 position = transform.Position;
			MotionType motionType = entity.GetComponent<PhysicsComponent>().motionType;
			Vector3 extents = Vector3::One;
			if (entity.HasComponent<MeshComponent>())
			{
				extents = entity.GetComponent<MeshComponent>().GetExtents();
			}
			auto halfExtents = (extents * transform.Scale) /2;
			halfExtents.x = std::max(halfExtents.x, 0.05f);
			halfExtents.y = std::max(halfExtents.y, 0.05f);
			halfExtents.z = std::max(halfExtents.z, 0.05f);
			phyEngine->RegisterBody(entity.GetId(), halfExtents, position, transform.EulerAngles, motionType);
		}

		if (entity.HasComponent<EnableComponent>()
			&& entity.GetComponent<EnableComponent>().Enabled)
		{ }
	}

	phyEngine->Update(fixedDeltaTime);

	for (Entity& entity : GetEntities())
	{
		auto phyComponent = entity.GetComponent<PhysicsComponent>();
		if (phyComponent.motionType != STATIC)
		{
			auto& transformComponent = entity.GetComponent<TransformComponent>();
			Vector3 newPosition = phyEngine->GetPosition(entity.GetId());
			transformComponent.Position = newPosition;
		}
	}
}