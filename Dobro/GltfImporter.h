#pragma once
#include "IModelImporter.h"

class GltfImporter : public IModelImporter
{
public:
	virtual std::vector<Mesh> Load(std::string path);

private:
	std::vector<Mesh> meshes;
};

