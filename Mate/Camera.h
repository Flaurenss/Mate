#pragma once
#include <Vector.h>
#include <Matrix.h>

enum MOVEMENT
{
	FORWARD,
	BACKWARD,
	RIGHT,
	LEFT,
	SHIFT
};

class Camera
{
public:
	Camera(Vector3 position, Vector3 worldUp, bool freeCamera = false);

	Matrix4 GetViewMatrix();

	void SetPosition(Vector3 position);
	
	void SetForward(Vector3 forward);
	Vector3 GetForward();

	void SetUp(Vector3 up);
	Vector3 GetUp();

	void SetRight(Vector3 right);
	Vector3 GetRight();

private:
	// Enter fields
	Vector3 Position;

	// Post calculated fields
	Vector3 Forward;
	Vector3 Up;
	Vector3 Right;
};

