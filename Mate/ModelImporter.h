#pragma once
#include <string>
#include "Mesh.h"
#include <memory>
#include <vector>
#include "GltfImporter.h"
#include "FbxImporterExperimental.h"

class ModelImporter
{
public:
	ModelImporter();
	~ModelImporter() = default;

	std::shared_ptr<Model> Load(const std::string& modelFilePath) const;
private:
	std::unique_ptr<GltfImporter> gltfImporter;
	//std::unique_ptr<FbxImporterExperimental> fbxImporter;

	IModelImporter* SelectImporter(const std::string& modelFilePath) const;
	bool EndsWith(const std::string& value, const std::string& ending) const;
};

