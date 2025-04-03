#pragma once
#include "Component.h"
#include <Matrix.h>

class TransformComponent : public Component
{
private:
	Matrix4 transform;
};

