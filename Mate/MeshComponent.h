#pragma once
#include "Component.h"
#include "Model.h"

class MeshComponent
{
public:
	MeshComponent(const std::string modelId);
	const std::string& GetModelId() const;

	Vector3 GetExtents() const;
	Vector3 GetCenter() const;
	Vector3 GetMin() const;

private:
	std::string modelId;
};

