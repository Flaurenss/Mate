#include "Model.h"
#include "ufbx.h"
#include "GltfImporter.h"
#include "stb_image.h"

Model::Model(const std::string& path)
{
    //modelImporter = new FbxImporterExperimental();
    modelImporter = new GltfImporter();
    meshes = modelImporter->Load(path);
}

Model::Model(Mesh mesh)
{
    meshes.push_back(mesh);
    modelImporter = nullptr;
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