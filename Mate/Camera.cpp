#include "Camera.h"
#include <MathUtils.h>
#include <corecrt_math.h>

Camera::Camera(Vector3 position, Vector3 up, bool freeCamera) : 
    Position(position),
	Forward(Vector3(0, 0, -1)),
	Up(Vector3::Up)
{ }

Matrix4 Camera::GetViewMatrix()
{
	return Matrix4::lookAt(Position, Position + Forward, Up);
}

void Camera::SetPosition(Vector3 position)
{
	Position = position;
}

void Camera::SetForward(Vector3 forward)
{
	Forward = forward;
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