#include "TransformComponent.h"

TransformComponent::TransformComponent() :
	TransformComponent(Vector3(0, 0, 0), Vector3(1, 1, 1), Vector3(0, 0, 0))
{ }

TransformComponent::TransformComponent(Vector3 position, Vector3 rotation, Vector3 scale)
{
	transform = Matrix4::transform(position, rotation, scale);
}
