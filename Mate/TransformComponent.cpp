#include "TransformComponent.h"

TransformComponent::TransformComponent() :
	TransformComponent(Vector3(0, 0, 0), Vector3(1, 1, 1), Vector3(1, 1, 1))
{ }

TransformComponent::TransformComponent(Vector3 position, Vector3 rotation, Vector3 scale)
{
    Matrix4 initTransform = Matrix4();

    initTransform.scale(scale);

    initTransform.rotate(rotation.z, Vector3(0, 0, 1));
    initTransform.rotate(rotation.y, Vector3(0, 1, 0));
    initTransform.rotate(rotation.x, Vector3(1, 0, 0));

    initTransform.translate(position);

    transform = initTransform;
}

Matrix4& TransformComponent::GetTransform()
{
    return transform;
}

void TransformComponent::Translate(Vector3 position)
{
    transform.translate(position);
}

void TransformComponent::Rotate(float angle, Vector3 axis)
{
    transform.rotate(angle, axis);
}

void TransformComponent::Scale(Vector3 scale)
{
    transform.scale(scale);
}
