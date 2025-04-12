#include "CameraComponent.h"
#include "Logger.h"

CameraComponent::CameraComponent() : CameraComponent(new Camera(Vector3(0.0f, 2.0f, 2.5f), Vector3::Up))
{ }

CameraComponent::CameraComponent(Camera* camera)
{
	Fov = 45.0f;
	Near = 0.1f;
	Far = 100.0f;
	this->camera = camera;
}

void CameraComponent::SetForward(Vector3 target)
{
	if (camera)
	{
		camera->SetForward(target);
	}
}

Matrix4 CameraComponent::GetViewMatrix()
{
	if (camera)
	{
		return camera->GetViewMatrix();
	}
	else
	{
		Logger::Err("Camera is not initialized in CameraComponent.");
	}
}
