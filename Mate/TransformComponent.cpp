#include "TransformComponent.h"

TransformComponent::TransformComponent() :
	TransformComponent(Vector3(0, 0, 0), Vector3(1, 1, 1), Vector3(1, 1, 1))
{ }

TransformComponent::TransformComponent(Vector3 position, Vector3 rotation, Vector3 scale) :
    position(position),
    eulerAngles(rotation),
    scale(scale) 
{ }

Matrix4 TransformComponent::GetTransform() const
{
    Matrix4 mScale;
    mScale.scale(scale);

    Matrix4 mRotX;
    mRotX.rotate(eulerAngles.x, Vector3::Right);
    Matrix4 mRotY;
    mRotY.rotate(eulerAngles.y, Vector3::Up);
    Matrix4 mRotZ;
    mRotZ.rotate(eulerAngles.z, Vector3::Forward);
    Matrix4 rotationMatrix = mRotZ * mRotY * mRotX;

    Matrix4 mTrans;
    mTrans.translate(position);

    return mTrans * rotationMatrix * mScale;
}

void TransformComponent::SetPosition(Vector3 newPosition)
{
    position = newPosition;
}

void TransformComponent::Translate(Vector3 delta)
{
    position += delta;
}

void TransformComponent::Rotate(Vector3 deltaDegrees)
{
    eulerAngles += deltaDegrees;
}

void TransformComponent::Scale(Vector3 newScale)
{
    scale = newScale;
}

void TransformComponent::Scale(float newScale)
{
    scale = Vector3(newScale, newScale, newScale);
}
