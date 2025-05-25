#pragma once
#include <Mate.h>

class CameraUtils
{
public:
	void ManageFreeCamera(
		CameraComponent& cameraComponent,
		TransformComponent& cameraTransform,
		float deltaTime);
private:
	bool firstMouse = true;
	float yaw = 45.0f;
	float pitch = 0;
	float lastX = 0;
	float lastY = 0;
};

