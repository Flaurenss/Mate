#pragma once
#include "Component.h"
#include "Model.h"

class MeshComponent
{
public:
	MeshComponent(std::string modelPath);
	MeshComponent(Model* model);
	Model& GetModel();
private:
	Model* model;
};

