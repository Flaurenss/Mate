#pragma once
#include <vector>
#include "Model.h"

class IModelImporter
{
public:
	virtual std::shared_ptr<Model> Load(const std::string& path) = 0;
};