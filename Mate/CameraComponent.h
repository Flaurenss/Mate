#pragma once
#include "Camera.h"

class CameraComponent
{
public:
	float Fov;
	float Near;
	float Far;

	CameraComponent();
	CameraComponent(Camera* camera);

	Matrix4 GetViewMatrix();
	void SetForward(Vector3 target);

private:
	Camera* camera;
};

