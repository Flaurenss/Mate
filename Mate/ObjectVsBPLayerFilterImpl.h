#pragma once
#include "CommonPhysics.h"

/// <summary>
/// Class that determines if an object layer can collide with a broadphase layer
/// </summary>
class ObjectVsBPLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter
{
public:
	virtual bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override
	{
		switch (inLayer1)
		{
		case Layers::NON_MOVING:
			return inLayer2 == BroadPhaseLayers::MOVING;
		case Layers::MOVING:
			return inLayer2 != BroadPhaseLayers::NON_COLLIDING;
		case Layers::NON_COLLIDING:
			return false;
		default:
			return false;
		}
	}
};

