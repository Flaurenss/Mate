#pragma once
#include <vector>
#include "Mesh.h"

class IModelImporter
{
public:
	virtual std::vector<std::shared_ptr<Mesh>> Load(const std::string& path) = 0;
};

