#pragma once
#include "Camera.h"

class CameraComponent
{
public:
	float Fov;
	float Near;
	float Far;

	CameraComponent(bool freeCamera = false);
	CameraComponent(Camera* camera);

	Matrix4 GetViewMatrix();
	void SetPosition(Vector3 position);
	void SetForward(Vector3 target);
	Vector3 GetForward();
	
	void SetUp(Vector3 up);
	Vector3 GetUp();

	void SetRight(Vector3 right);
	Vector3 GetRight();

private:
	Camera* camera;
};

