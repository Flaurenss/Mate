#pragma once
#include "Component.h"
#include "Matrix.h"

class TransformComponent
{
public:
	Vector3 Position;
	Vector3 EulerAngles;
	Vector3 Scale;

	TransformComponent();
	TransformComponent(Vector3 position);
	TransformComponent(Vector3 position, Vector3 scale, Vector3 rotation);

	Matrix4 GetTransform() const;

	void SetPosition(Vector3 position);

	void Translate(Vector3 position);
	void Rotate(Vector3 axis);
	void DoScale(Vector3 newScale);
	void DoScale(float newScale);

	Vector3 GetForward() const;;
	Vector3 GetUp() const;;
	Vector3 GetRight() const;;
private:
	Matrix4 transform;

	Matrix4 GetRotationMatrix() const;
};

