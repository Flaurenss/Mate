#include "CameraComponent.h"

CameraComponent::CameraComponent()
{
	camera = new Camera(Vector3(0.0f, 0.0f, 3.0f), Vector3(0.0f, 1.0f, 0.0f));
}

Camera& CameraComponent::GetCamera()
{
	return *camera;
}
