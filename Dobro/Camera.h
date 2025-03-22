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
	Camera(Vector3 position, Vector3 worldUp);

	void ProcessMouseMovement(float xOffset, float yOffset);
	void ProcessKeyboardMovement(MOVEMENT movement, float deltaTime);

	Matrix4 GetViewMatrix();
private:
	// Enter fields
	Vector3 Position;
	Vector3 WorldUp;

	// Post calculated fields
	Vector3 Forward;
	Vector3 Up;
	Vector3 Right;

	float yaw = -90.0f;
	float pitch = 0.0f;
	float speed = 2.5f;
	float sensitivity = 0.1f;

	void UpdateCameraVectors();
};

