#include "CameraUtils.h"

CameraUtils::CameraUtils(Entity theCamera) : camera(theCamera)
{ }

void CameraUtils::ManageFreeCamera(float deltaTime)
{
    float xPos = Input::MousePosition.x;
    float yPos = Input::MousePosition.y;
    float sensitivity = 0.1f;

    auto& cameraTransform = camera.GetComponent<TransformComponent>();

    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xoffset = lastX - xPos;
    float yoffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    Vector3 currentEuler = cameraTransform.EulerAngles;
    currentEuler.y = yaw;
    currentEuler.x = pitch;
    currentEuler.z = 0;
    cameraTransform.EulerAngles = currentEuler;

    float speed = deltaTime * 5.0f;
    Vector3 direction = Vector3::Zero;
    Vector3 transformForward = cameraTransform.GetForward();
    Vector3 transformRight = cameraTransform.GetRight();

    if (Input::GetKey(KeyCode::W)) direction += transformForward;
    if (Input::GetKey(KeyCode::S)) direction -= transformForward;
    if (Input::GetKey(KeyCode::A)) direction -= transformRight;
    if (Input::GetKey(KeyCode::D)) direction += transformRight;

    cameraTransform.Translate(direction.normalize() * speed);
}
