#pragma once
#include "Camera.h"

class CameraComponent
{
public:
	CameraComponent();
	CameraComponent(Camera* camera);

	Camera& GetCamera();

private:
	Camera* camera;
};

