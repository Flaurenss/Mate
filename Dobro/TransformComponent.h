#pragma once
#include "Component.h"
#include <Matrix.h>

class TransformComponent : public Component<TransformComponent>
{
public:
	TransformComponent();
	TransformComponent(Vector3 position, Vector3 scale, Vector3 rotation);
private:
	Matrix4 transform;
};

