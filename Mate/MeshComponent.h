#pragma once
#include "Component.h"
#include "Model.h"

class MeshComponent
{
public:
	MeshComponent(std::string modelPath);
	Model& GetModel();
private:
	Model* model;
};

