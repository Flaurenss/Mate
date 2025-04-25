#include "MeshComponent.h"

MeshComponent::MeshComponent(std::vector<std::shared_ptr<Mesh>> meshes)
{
	model = new Model(meshes);
}

MeshComponent::MeshComponent(Model* newModel)
{
	model = newModel;
}

Model& MeshComponent::GetModel()
{
	return *model;
}

Vector3 MeshComponent::GetExtents()
{
	return model->GetExtents();
}
