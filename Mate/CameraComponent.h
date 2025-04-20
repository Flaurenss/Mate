#pragma once

class CameraComponent
{
public:
	float Fov;
	float Near;
	float Far;

	CameraComponent();
	CameraComponent(float fov, float near, float far);
};

