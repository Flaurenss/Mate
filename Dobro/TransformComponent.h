#pragma once
#include "Component.h"
#include <Matrix.h>

class TransformComponent : public Component<TransformComponent>
{
private:
	Matrix4 transform;
};

