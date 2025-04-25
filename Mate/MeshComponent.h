#pragma once
#include "Component.h"
#include "Model.h"

class MeshComponent
{
public:
	MeshComponent(std::vector<std::shared_ptr<Mesh>> meshes);
	MeshComponent(Model* model);
	Model& GetModel();
	Vector3 GetExtents();
private:
	Model* model;
};

