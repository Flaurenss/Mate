#include "Camera.h"

Camera::Camera(Vector3 position, Vector3 up) : 
    Position(position),
	WorldUp(up),
	Forward(Vector3(0, 0, -1))
{
    // calculate the new Front vector
    Forward = Forward.normalize();
    Right = Vector3::cross(Forward, WorldUp).normalize();  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = Vector3::cross(Right, Forward);
}

Matrix4 Camera::GetViewMatrix()
{
    return Matrix4::lookAt(Position, Position + Forward, Up);
}
