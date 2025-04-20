#include "CameraComponent.h"
#include "Logger.h"

CameraComponent::CameraComponent() :
	CameraComponent(45.0f, 0.1f, 100.0f)
{ }

CameraComponent::CameraComponent(float fov, float near, float far)
{
	Fov = fov;
	Near = near;
	Far = far;
}