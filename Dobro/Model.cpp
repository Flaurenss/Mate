#include "Model.h"
#include "ufbx.h"
#include "stb_image.h"

Model::Model(const std::string& path)
{
    modelImporter = new FbxImporterExperimental();
    meshes = modelImporter->Load(path);
}

Model::~Model()
{
    delete modelImporter;
    modelImporter = nullptr;
}

void Model::Draw(Shader& shader)
{
	for (auto i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shader);
	}
}