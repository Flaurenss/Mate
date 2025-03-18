#pragma once
#include <Vector.h>
#include <Matrix.h>

class Camera
{
public:
	Camera(Vector3 position, Vector3 worldUp);

	Matrix4 GetViewMatrix();
private:
	// Enter fields
	Vector3 Position;
	Vector3 WorldUp;

	// Post calculated fields
	Vector3 Forward;
	Vector3 Up;
	Vector3 Right;
};

