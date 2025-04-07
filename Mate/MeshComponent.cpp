#include "MeshComponent.h"

MeshComponent::MeshComponent(std::string modelPath)
{
	Model newModel(modelPath);
	model = new Model(modelPath);
}

MeshComponent::MeshComponent(Model* newModel)
{
	model = newModel;
}

Model& MeshComponent::GetModel()
{
	return *model;
}
