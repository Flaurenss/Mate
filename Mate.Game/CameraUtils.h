#pragma once
#include <Mate.h>

class CameraUtils
{
public:
	CameraUtils(Entity theCamera);
	void ManageFreeCamera(
		float deltaTime);
private:
	Entity camera;

	bool firstMouse = true;
	float yaw = 45.0f;
	float pitch = 0;
	float lastX = 0;
	float lastY = 0;
};

