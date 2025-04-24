#pragma once
#include "MateMotionTypes.h"

struct PhysicsComponent
{
	MotionType motionType;

	PhysicsComponent() : motionType(STATIC) {}
	PhysicsComponent(MotionType type) : motionType(type) {}
};
