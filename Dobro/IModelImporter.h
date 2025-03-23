#pragma once
#include <vector>
#include "Mesh.h"

class IModelImporter
{
public:
	virtual std::vector<Mesh> Load(std::string path) = 0;
};

