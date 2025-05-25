#include "MateContactListener.h"
#include "PhysicsEngine.h"

MateContactListener::MateContactListener(PhysicsEngine* engine)
{
	physicsEngine = engine;
}

JPH::ValidateResult MateContactListener::OnContactValidate(const JPH::Body& inBody1, const JPH::Body& inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult& inCollisionResult)
{
	if (!inBody1.IsActive() || !inBody2.IsActive())
	{
		// If any of them are inactive we reject the collision:
		return JPH::ValidateResult::RejectAllContactsForThisBodyPair;
	}

	return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
}

void MateContactListener::OnContactAdded(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings)
{
	//Logger::Log("A contact was added");
	int id1 = static_cast<int>(inBody1.GetUserData());
	int id2 = static_cast<int>(inBody2.GetUserData());

	if (physicsEngine->IsRegistered(id1) && physicsEngine->IsRegistered(id2))
	{
		physicsEngine->AddNewCollision(id1, id2);
	}
}

void MateContactListener::OnContactPersisted(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings)
{
	//Logger::Log("A contact was persisted");
}

void MateContactListener::OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair)
{
	//Logger::Log("A contact was removed");
}
