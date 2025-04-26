#pragma once
#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include "Logger.h"

/// <summary>
/// Basic Contact Listener class from Jolt Samples
/// </summary>
class MateContactListener : public JPH::ContactListener
{
	// See: ContactListener
	virtual JPH::ValidateResult	OnContactValidate(const JPH::Body& inBody1, const JPH::Body& inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult& inCollisionResult) override
	{
		// Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
		return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
	}

	virtual void OnContactAdded(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override
	{
		Logger::Log("A contact was added");
	}

	virtual void OnContactPersisted(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override
	{
		Logger::Log("A contact was persisted");
	}

	virtual void OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair) override
	{
		Logger::Log("A contact was removed");
	}
};

