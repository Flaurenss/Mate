#pragma once
#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>

#include "MateMotionTypes.h"
#include "PhysicLayer.h"


// Layer that objects can be in, determines which other objects it can collide with
// Typically you at least want to have 1 layer for moving bodies and 1 layer for static bodies, but you can have more
// layers if you want. E.g. you could have a layer for high detail collision (which is not used by the physics simulation
// but only if you do collision testing).
namespace Layers
{
	static constexpr JPH::ObjectLayer NON_MOVING = 0;
	static constexpr JPH::ObjectLayer MOVING = 1;
	static constexpr JPH::ObjectLayer NON_COLLIDING = 2;
	static constexpr JPH::ObjectLayer NUM_LAYERS = 3;
};

// Each broadphase layer results in a separate bounding volume tree in the broad phase. You at least want to have
// a layer for non-moving and moving objects to avoid having to update a tree full of static objects every frame.
// You can have a 1-on-1 mapping between object layers and broadphase layers (like in this case) but if you have
// many object layers you'll be creating many broad phase trees, which is not efficient. If you want to fine tune
// your broadphase layers define JPH_TRACK_BROADPHASE_STATS and look at the stats reported on the TTY.
namespace BroadPhaseLayers
{
	static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
	static constexpr JPH::BroadPhaseLayer MOVING(1);
	static constexpr JPH::BroadPhaseLayer NON_COLLIDING(2);
	static constexpr unsigned int NUM_LAYERS(3);
};

inline JPH::EMotionType MotionTypeToEMotionType(MotionType motionType)
{
	switch (motionType)
	{
		case MotionType::STATIC:
			return JPH::EMotionType::Static;
		case MotionType::DYNAMIC:
			return JPH::EMotionType::Dynamic;
		case MotionType::KINEMATIC:
			return JPH::EMotionType::Kinematic;
		default:
			return JPH::EMotionType::Static;
	}
}

inline JPH::ObjectLayer LayerToObjectLayer(PhysicLayer layer)
{
	switch (layer)
	{
	case PhysicLayer::MOVING:
		return Layers::MOVING;
	case PhysicLayer::NON_MOVING:
		return Layers::NON_MOVING;
	case PhysicLayer::NON_COLLIDING:
		return Layers::NON_COLLIDING;
	default:
		return Layers::MOVING;
	}
}