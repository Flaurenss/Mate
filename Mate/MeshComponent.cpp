#include "MeshComponent.h"

MeshComponent::MeshComponent(std::string modelPath)
{
	model = new Model(modelPath);
}
