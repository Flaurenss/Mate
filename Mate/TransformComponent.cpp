#include "TransformComponent.h"

TransformComponent::TransformComponent() :
	TransformComponent(Vector3(0, 0, 0), Vector3(1, 1, 1), Vector3(1, 1, 1))
{ }

TransformComponent::TransformComponent(Vector3 position, Vector3 rotation, Vector3 scale) :
    Position(position),
    EulerAngles(rotation),
    Scale(scale) 
{ }

Matrix4 TransformComponent::GetTransform() const
{
    Matrix4 mScale;
    mScale.scale(Scale);

    Matrix4 mRotX;
    mRotX.rotate(EulerAngles.x, Vector3::Right);
    Matrix4 mRotY;
    mRotY.rotate(EulerAngles.y, Vector3::Up);
    Matrix4 mRotZ;
    mRotZ.rotate(EulerAngles.z, Vector3::Forward);
    Matrix4 rotationMatrix = mRotZ * mRotY * mRotX;

    Matrix4 mTrans;
    mTrans.translate(Position);

    return mTrans * rotationMatrix * mScale;
}

void TransformComponent::SetPosition(Vector3 newPosition)
{
    Position = newPosition;
}

void TransformComponent::Translate(Vector3 delta)
{
    Position += delta;
}

void TransformComponent::Rotate(Vector3 deltaDegrees)
{
    EulerAngles += deltaDegrees;
}

void TransformComponent::DoScale(Vector3 newScale)
{
    Scale = newScale;
}

void TransformComponent::DoScale(float newScale)
{
    Scale = Vector3(newScale, newScale, newScale);
}
