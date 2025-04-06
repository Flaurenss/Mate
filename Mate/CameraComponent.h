#pragma once
#include "Camera.h"

class CameraComponent
{
public:
	CameraComponent();

	Camera& GetCamera();

private:
	Camera* camera;
};

