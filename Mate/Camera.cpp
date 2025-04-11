#include "Camera.h"
#include <MathUtils.h>
#include <corecrt_math.h>

Camera::Camera(Vector3 position, Vector3 up) : 
    Position(position),
	WorldUp(up),
	Forward(Vector3(0, 0, -1))
{
    // calculate the new Front vector
	UpdateCameraVectors();
}

void Camera::ProcessKeyboardMovement(MOVEMENT movement, float deltaTime)
{
	float velocity = speed * deltaTime;
	if (movement == FORWARD)
		Position += Forward * velocity;
	if (movement == BACKWARD)
		Position -= Forward * velocity;
	if (movement == LEFT)
		Position -= Right * velocity;
	if (movement == RIGHT)
		Position += Right * velocity;
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset)
{
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (pitch > 80.0f)
		pitch = 80.0f;
	if (pitch < -80.0f)
		pitch = -80.0f;
	
	UpdateCameraVectors();
}

Matrix4 Camera::GetViewMatrix()
{
	//Position + Forward
	return Matrix4::lookAt(Vector3(0, 5, 5), Vector3(0, 0, 0), Vector3(0, 1, 0));
    //return Matrix4::lookAt(Position, Position + Forward, Up);
}

void Camera::SetForward(Vector3 target)
{
	Forward = target - Position;
}

void Camera::UpdateCameraVectors()
{
	Vector3 newForward;
	newForward.x = cosf(MathUtils::radians(yaw)) * cosf(MathUtils::radians(pitch));
	newForward.y = sinf(MathUtils::radians(pitch));
	newForward.z = sinf(MathUtils::radians(yaw)) * cosf(MathUtils::radians(pitch));

	Forward = newForward.normalize();
	Right = Vector3::cross(Forward, WorldUp).normalize();  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = Vector3::cross(Right, Forward);
}
