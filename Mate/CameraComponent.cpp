#include "CameraComponent.h"

CameraComponent::CameraComponent()
{
	camera = new Camera(Vector3(0.0f, 0.5f, 1), Vector3(0.0f, 1.0f, 0.0f));
}

CameraComponent::CameraComponent(Camera* camera)
{
	this->camera = camera;
}

Camera& CameraComponent::GetCamera()
{
	return *camera;
}
