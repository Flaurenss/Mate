#include "Camera.h"
#include <MathUtils.h>
#include <corecrt_math.h>

Camera::Camera(Vector3 position, Vector3 up, bool freeCamera) : 
    Position(position),
	WorldUp(up),
	Forward(Vector3(0, 0, -1)),
	freeCamera(freeCamera)
{
 //   // calculate the new Front vector
	//if (freeCamera)
	//{
	//	UpdateCameraVectors();
	//}
}

void Camera::ProcessKeyboardMovement(MOVEMENT movement, float deltaTime)
{
	//float velocity = speed * deltaTime;
	//if (movement == FORWARD)
	//	Position += Forward * velocity;
	//if (movement == BACKWARD)
	//	Position -= Forward * velocity;
	//if (movement == LEFT)
	//	Position -= Right * velocity;
	//if (movement == RIGHT)
	//	Position += Right * velocity;
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset)
{
	//xOffset *= sensitivity;
	//yOffset *= sensitivity;

	//yaw += xOffset;
	//pitch += yOffset;

	//if (pitch > 80.0f)
	//	pitch = 80.0f;
	//if (pitch < -80.0f)
	//	pitch = -80.0f;
	//
	//UpdateCameraVectors();
}

Matrix4 Camera::GetViewMatrix()
{
	return Matrix4::lookAt(Position, Position + Forward, WorldUp);
}

void Camera::SetPosition(Vector3 position)
{
	Position = position;
}

void Camera::SetForward(Vector3 forward)
{
	//Forward = target - Position;
	Forward = forward;
}

void Camera::SetFreeCamera(bool activate)
{
	freeCamera = activate;
}

Vector3 Camera::GetForward()
{
	return Forward;
}

void Camera::SetUp(Vector3 up)
{
	Up = up;
}

Vector3 Camera::GetUp()
{
	return Up;
}

void Camera::SetRight(Vector3 right)
{
	Right = right;
}

Vector3 Camera::GetRight()
{
	return Right;
}

void Camera::UpdateCameraVectors()
{
	//Vector3 newForward;
	//newForward.x = cosf(MathUtils::radians(yaw)) * cosf(MathUtils::radians(pitch));
	//newForward.y = sinf(MathUtils::radians(pitch));
	//newForward.z = sinf(MathUtils::radians(yaw)) * cosf(MathUtils::radians(pitch));

	//Forward = newForward.normalize();
	//Right = Vector3::cross(Forward, WorldUp).normalize();  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	//Up = Vector3::cross(Right, Forward);
}
