#pragma once
#include "Component.h"
#include "Matrix.h"

class TransformComponent
{
public:
	Vector3 position;
	Vector3 eulerAngles;
	Vector3 scale;

	TransformComponent();
	TransformComponent(Vector3 position, Vector3 scale, Vector3 rotation);

	Matrix4 GetTransform() const;

	void SetPosition(Vector3 position);

	void Translate(Vector3 position);
	void Rotate(Vector3 axis);
	void Scale(Vector3 scale);
private:
	Matrix4 transform;
};

