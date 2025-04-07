#pragma once
#include "Component.h"
#include "Matrix.h"

class TransformComponent
{
public:
	TransformComponent();
	TransformComponent(Vector3 position, Vector3 scale, Vector3 rotation);

	Matrix4& GetTransform();
	void Translate(Vector3 position);
	void Rotate(float angle, Vector3 axis);
	void Scale(Vector3 scale);
private:
	Matrix4 transform;
};

