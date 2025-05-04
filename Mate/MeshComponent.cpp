#include "MeshComponent.h"
#include "AssetManager.h"

MeshComponent::MeshComponent(const std::string modelId) : modelId(modelId)
{ }

const std::string& MeshComponent::GetModelId() const
{
	return modelId;
}

Vector3 MeshComponent::GetExtents() const
{
    auto model = AssetManager::GetInstance().GetModel(modelId);
    return model ? model->GetExtents() : Vector3::Zero;
}

Vector3 MeshComponent::GetCenter() const
{
    auto model = AssetManager::GetInstance().GetModel(modelId);
    return model ? model->GetCenter() : Vector3::Zero;
}

Vector3 MeshComponent::GetMin() const
{
    auto model = AssetManager::GetInstance().GetModel(modelId);
    return model? model->Min : Vector3::Zero;
}
