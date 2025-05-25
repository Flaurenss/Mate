#include "MateContactListener.h"
#include "PhysicsEngine.h"

MateContactListener::MateContactListener(PhysicsEngine* engine)
{
	physicsEngine = engine;
}

JPH::ValidateResult MateContactListener::OnContactValidate(const JPH::Body& inBody1, const JPH::Body& inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult& inCollisionResult)
{
	// Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
	/*auto data = physicsEngine->GetEntityPhysicsData(inBody1.GetUserData());
	auto phy = data.GetEntity().GetComponent<PhysicsComponent>();

	auto data2 = physicsEngine->GetEntityPhysicsData(inBody2.GetUserData());
	auto phy2 = data2.GetEntity().GetComponent<PhysicsComponent>();*/

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
	physicsEngine->AddNewCollision(
		static_cast<int>(inBody1.GetUserData()),
		static_cast<int>(inBody2.GetUserData()));
}

void MateContactListener::OnContactPersisted(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings)
{
	//Logger::Log("A contact was persisted");
}

void MateContactListener::OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair)
{
	//Logger::Log("A contact was removed");
}
